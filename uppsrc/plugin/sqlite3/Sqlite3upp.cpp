#include <Core/Core.h>
#include <Sql/Sql.h>
#include "lib/sqlite3.h"
#include "Sqlite3.h"

NAMESPACE_UPP

#define LLOG(x) // LOG(x)

class Sqlite3Connection : public SqlConnection, public Link<Sqlite3Connection> {
protected:
	virtual void        SetParam(int i, const Value& r);
	virtual bool        Execute();
	virtual int         GetRowsProcessed() const;
	virtual Value       GetInsertedId() const;
	virtual bool        Fetch();
	virtual void        GetColumn(int i, Ref f) const;
	virtual void        Cancel();
	virtual SqlSession& GetSession() const;
	virtual String      ToString() const;

	typedef Sqlite3Session::sqlite3 sqlite3;
	typedef Sqlite3Session::sqlite3_stmt sqlite3_stmt;

private:
	Sqlite3Session&  session;
	sqlite3*         db;
	Vector<Value>    param;

	sqlite3_stmt*    current_stmt;
	String           current_stmt_string;
	bool             got_first_row;
	bool             got_row_data;

	friend class Sqlite3Session;
	void             BindParam(int i, const Value& r);
	void             Reset();

public:
	Sqlite3Connection(Sqlite3Session& the_session, sqlite3 *the_db);
	virtual ~Sqlite3Connection();
};

Sqlite3Connection::Sqlite3Connection(Sqlite3Session& the_session, sqlite3 *the_db)
:	session(the_session), db(the_db), current_stmt(NULL), got_first_row(false), got_row_data(false)
{
	LinkBefore(&session.clink);
}

Sqlite3Connection::~Sqlite3Connection()
{
	Cancel();
	Unlink();
}

void Sqlite3Connection::Cancel()
{
	if (current_stmt) {
//		if (sqlite3_reset(current_stmt) != SQLITE_OK)
//			session.SetError(sqlite3_errmsg(db), "Resetting statement: " + current_stmt_string);
//		if (sqlite3_finalize(current_stmt) != SQLITE_OK)
//			session.SetError(sqlite3_errmsg(db), "Finalizing statement: "+ current_stmt_string);
		//this seems to be the correct way how to do error recovery...
		sqlite3_finalize(current_stmt);
		current_stmt = NULL;
		current_stmt_string.Clear();
		parse = true;
	}
}

void Sqlite3Connection::Reset()
{
	if(current_stmt && sqlite3_reset(current_stmt) != SQLITE_OK)
		session.SetError(sqlite3_errmsg(db), "Resetting statement: " + current_stmt_string);
}

void Sqlite3Connection::SetParam(int i, const Value& r)
{
	LLOG(Format("SetParam(%d,%s)",i,r.ToString()));
	param.At(i) = r;
}

void Sqlite3Connection::BindParam(int i, const Value& r) {
	if (IsNull(r))
		sqlite3_bind_null(current_stmt,i);
	else switch (r.GetType()) {
		case STRING_V:
		case WSTRING_V: {
			WString p = r;
			sqlite3_bind_text16(current_stmt,i,p,2*p.GetLength(),SQLITE_TRANSIENT);
			break;
		}
		case BOOL_V:
		case INT_V:
			sqlite3_bind_int(current_stmt, i, int(r));
			break;
		case INT64_V:
			sqlite3_bind_int64(current_stmt, i, int64(r));
			break;
		case DOUBLE_V:
			sqlite3_bind_double(current_stmt, i, double(r));
			break;
		case DATE_V: {
				Date d = r;
				String p = Format("%04d-%02d-%02d", d.year, d.month, d.day);
				sqlite3_bind_text(current_stmt,i,p,p.GetLength(),SQLITE_TRANSIENT);
			}
			break;
		case TIME_V: {
				Time t = r;
				String p = Format("%04d-%02d-%02d %02d:%02d:%02d",
						          t.year, t.month, t.day, t.hour, t.minute, t.second);
				sqlite3_bind_text(current_stmt,i,p,p.GetLength(),SQLITE_TRANSIENT);
			}
			break;
		default:
			NEVER();
	}
}

int ParseForArgs(const char* sqlcmd)
{
   int numargs = 0;
   const char* ptr = sqlcmd;
   while (*ptr)
      if(*ptr == '\'')
         while(*++ptr && (*ptr != '\'' || *++ptr && *ptr == '\''))
            ;
      else if(*ptr == '-' && *(ptr+1) == '-')
         while(*++ptr && *ptr != '\n' && *ptr != '\r')
            ;
      else if(*ptr == '/' && *(ptr+1) == '*')
      {
         ptr++;
         while(*++ptr && (*ptr != '*' || (*(ptr+1) && *(ptr+1) != '/')))
            ;
      }
      else if(*ptr++ == '?')
         ++numargs;
   return numargs;
}

bool Sqlite3Connection::Execute() {
	Cancel();
	if(statement.GetLength() == 0) {
		session.SetError("Empty statement", String("Preparing: ") + statement);
		return false;
	}
	if (SQLITE_OK != sqlite3_prepare(db,statement,statement.GetLength(),&current_stmt,NULL)) {
		LLOG("Sqlite3Connection::Compile(" << statement << ") -> error");
		session.SetError(sqlite3_errmsg(db), String("Preparing: ") + statement);
		return false;
	}
	current_stmt_string = statement;
	int nparams = ParseForArgs(current_stmt_string);
	ASSERT(nparams == param.GetCount());
	for (int i = 0; i < nparams; ++i)
		BindParam(i+1,param[i]);
	param.Clear();
	// Make sure that compiling the statement never fails.
	ASSERT(NULL != current_stmt);
	int retcode;
	dword ticks_start = GetTickCount();
	int sleep_ms = 1;
	do{
		retcode = sqlite3_step(current_stmt);
		if(retcode!=SQLITE_BUSY && retcode!=SQLITE_LOCKED) break;
		if(session.busy_timeout == 0) break;
		if(session.busy_timeout>0){
			if((int)(GetTickCount()-ticks_start)>session.busy_timeout){
				break;
			}
		}//else infinite retry
		if(retcode==SQLITE_LOCKED) sqlite3_reset(current_stmt);
		Sleep(sleep_ms);
		if(sleep_ms<128) sleep_ms += sleep_ms;
	}while(1);
	if ((retcode != SQLITE_DONE) && (retcode != SQLITE_ROW)) {
		session.SetError(sqlite3_errmsg(db), current_stmt_string);
		return false;
	}
	got_first_row = got_row_data = (retcode==SQLITE_ROW);
//	if (got_row_data) { // By WebChaot, 2009-01-15
		int numfields = sqlite3_column_count(current_stmt);
		info.SetCount(numfields);
		for (int i = 0; i < numfields; ++i) {
			SqlColumnInfo& field = info[i];
			field.name = sqlite3_column_name(current_stmt,i);
			String coltype = sqlite3_column_decltype(current_stmt,i);
			switch (sqlite3_column_type(current_stmt,i)) {
				case SQLITE_INTEGER:
					field.type = INT_V;
					break;
				case SQLITE_FLOAT:
					field.type = DOUBLE_V;
					break;
				case SQLITE_TEXT:
					if(coltype == "date")
						field.type = DATE_V;
					else
					if(coltype == "datetime")
						field.type = TIME_V;
					else
						field.type = WSTRING_V;
					break;
				case SQLITE_NULL:
					if(coltype == "date")
						field.type = DATE_V;
					else
					if(coltype == "datetime")
						field.type = TIME_V;
					else
					if(coltype.Find("char") >= 0 || coltype.Find("text") >= 0 )
						field.type = WSTRING_V;
					else
					if(coltype.Find("integer") >= 0)
						field.type = INT_V;
					else
					if(coltype.Find("real") >= 0)
						field.type = DOUBLE_V;
					else
						field.type = VOID_V;
					break;
				case SQLITE_BLOB:
					field.type = STRING_V;
					break;
				default:
					NEVER();
					break;
			}
		}
//	}
	return true;
}

int Sqlite3Connection::GetRowsProcessed() const
{
	LLOG("GetRowsProcessed()");
	return sqlite3_changes(db);
}

Value Sqlite3Connection::GetInsertedId() const
{
	LLOG("GetInsertedId()");
	return sqlite3_last_insert_rowid(db);
}

bool Sqlite3Connection::Fetch() {
	ASSERT(NULL != current_stmt);
	if (!got_row_data)
		return false;
	if (got_first_row) {
		got_first_row = false;
		return true;
	}
	ASSERT(got_row_data);
	int retcode = sqlite3_step(current_stmt);
	if ((retcode != SQLITE_DONE) && (retcode != SQLITE_ROW))
		session.SetError(sqlite3_errmsg(db), String("Fetching prepared statement: ")+current_stmt_string );
	got_row_data = (retcode==SQLITE_ROW);
	return got_row_data;
}

void Sqlite3Connection::GetColumn(int i, Ref f) const {
	ASSERT(NULL != current_stmt);
	if(i == -1) {
		f = Value(sqlite3_last_insert_rowid(db));
		return;
	}

	ASSERT(got_row_data);
	String coltype;
	const char *s = sqlite3_column_decltype(current_stmt,i);
	if(s) coltype = ToLower(s);
	switch (sqlite3_column_type(current_stmt,i)) {
		case SQLITE_INTEGER:
			f = sqlite3_column_int64(current_stmt,i);
			break;
		case SQLITE_FLOAT:
			f = sqlite3_column_double(current_stmt,i);
			break;
		case SQLITE_TEXT:
			if(coltype == "date" || f.GetType() == DATE_V){
				const char *s = (const char *)sqlite3_column_text(current_stmt, i);
				if(strlen(s) >= 10)
					f = Value(Date(atoi(s), atoi(s + 5), atoi(s + 8)));
				else
					f = Null;
			}
			else
			if(coltype == "datetime" || f.GetType() == TIME_V) {
				const char *s = (const char *)sqlite3_column_text(current_stmt, i);
				if(strlen(s) >= 19)
					f = Value(Time(atoi(s), atoi(s + 5), atoi(s + 8), atoi(s + 11), atoi(s + 14), atoi(s + 17)));
				else
				if(strlen(s) >= 10)
					f = Value(ToTime(Date(atoi(s), atoi(s + 5), atoi(s + 8))));
				else
					f = Null;
			}
			else
				f = Value(WString((const wchar*)sqlite3_column_text16(current_stmt,i)));
			break;
		case SQLITE_NULL:
			f = Null;
			break;
		case SQLITE_BLOB:
			f = Value(String( (const byte*)sqlite3_column_blob(current_stmt,i),
			                  sqlite3_column_bytes(current_stmt,i)                ));
			break;
		default:
			NEVER();
			break;
	}
	return;
}
SqlSession& Sqlite3Connection::GetSession() const { return session; }
String Sqlite3Connection::ToString() const {
	return statement;
}

//////////////////////////////////////////////////////////////////////


bool Sqlite3Session::Open(const char* filename) {
	// Only open db once.
	ASSERT(NULL == db);
	current_filename = filename;
	// By default, sqlite3 associates the opened db with "main.*"
	// However, using the ATTACH sql command, it can connect to more databases.
	// I don't know how to get the list of attached databases from the API
	current_dbname = "main";
	if(SQLITE_OK == sqlite3_open(filename, &db))
		return true;
	if(db) {
		sqlite3_close(db);
		db = NULL;
	}
	return false;
}
void Sqlite3Session::Close() {
	if (NULL != db) {
		int retval;
#ifndef flagNOAPPSQL
		if(SQL.IsOpen() && &SQL.GetSession() == this)
			SQL.Cancel();
#endif
		retval = sqlite3_close(db);
		// If this function fails, that means that some of the
		// prepared statements have not been finalized.
		// See lib/sqlite3.h:91
		ASSERT(SQLITE_OK == retval);
		db = NULL;
	}
}
SqlConnection *Sqlite3Session::CreateConnection() {
	return new Sqlite3Connection(*this, db);
}

int Sqlite3Session::SqlExecRetry(const char *sql)
{
	ASSERT(NULL != sql);
	ASSERT(0 != *sql);
	int retcode;
	dword ticks_start = GetTickCount();
	int sleep_ms = 1;
	do{
		retcode = sqlite3_exec(db,sql,NULL,NULL,NULL);
		if(retcode!=SQLITE_BUSY && retcode!=SQLITE_LOCKED) break;
		if(busy_timeout == 0) break;
		if(busy_timeout>0){
			if((int)(GetTickCount()-ticks_start)>busy_timeout) break;
		}//else infinite retry
		Sleep(sleep_ms);
		if(sleep_ms<128) sleep_ms += sleep_ms;
	}while(1);
	return retcode;
}

void Sqlite3Session::Reset()
{
	for(Sqlite3Connection *s = clink.GetNext(); s != &clink; s = s->GetNext())
		s->Reset();
}

void Sqlite3Session::Cancel()
{
	for(Sqlite3Connection *s = clink.GetNext(); s != &clink; s = s->GetNext())
		s->Cancel();
}

Sqlite3Session::Sqlite3Session()
{
	db = NULL;
	Dialect(SQLITE3);
	busy_timeout = 0;
}

Sqlite3Session::~Sqlite3Session()
{
	Close();
}

void Sqlite3Session::Begin() {
	static const char begin[] = "BEGIN;";
	if(trace)
		*trace << begin << "\n";
	Reset();
	if(SQLITE_OK != SqlExecRetry(begin))
		SetError(sqlite3_errmsg(db), begin);
}

void Sqlite3Session::Commit() {
	Cancel();
	static const char commit[] = "COMMIT;";
	if(trace)
		*trace << commit << "\n";
	Reset();
	if(SQLITE_OK != SqlExecRetry(commit))
		SetError(sqlite3_errmsg(db), commit);
}

void Sqlite3Session::Rollback() {
	Cancel();
	static const char rollback[] = "ROLLBACK;";
	if(trace)
		*trace << rollback << "\n";
	if(SQLITE_OK != SqlExecRetry(rollback))
		SetError(sqlite3_errmsg(db), rollback);
}
Vector<String> Sqlite3Session::EnumDatabases() {
	Vector<String> out;
	Sql sql(*this);
	sql.Execute("PRAGMA database_list;");
	while (sql.Fetch())
		out.Add(sql[1]);  // sql[1] is database name, sql[2] is filename
	return out;
}

Vector<String> Sqlite3Session::EnumTables(String database) {
	Vector<String> out;
	String dbn=database;
	if(dbn.IsEmpty()) dbn=current_dbname; // for backward compatibility
	Sql sql(*this);
	sql.Execute("SELECT name FROM "+dbn+".sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%' ORDER BY 1;");
	while (sql.Fetch())
		out.Add(sql[0]);
	return out;
}

Vector<String> Sqlite3Session::EnumViews(String database) {
	Vector<String> out;
	String dbn=database;
	if(dbn.IsEmpty()) dbn=current_dbname;
	Sql sql(*this);
	sql.Execute("SELECT name FROM "+dbn+".sqlite_master WHERE type='view' AND name NOT LIKE 'sqlite_%' ORDER BY 1;");
	while (sql.Fetch())
		out.Add(sql[0]);
	return out;
}

int Sqlite3Session::GetTransactionLevel() const
{
	int autocommit = sqlite3_get_autocommit(db);
	return (autocommit ? 0 : 1);
}

//////////////////////////////////////////////////////////////////////////

Vector<SqlColumnInfo> Sqlite3Session::EnumColumns(String database, String table) {
	Vector<SqlColumnInfo> out;
	SqlColumnInfo info;
	String ColType;
	Sql sql(*this);
	sql.Execute("PRAGMA table_info("+table+");");
	while (sql.Fetch()) {
		info.width = info.scale = info.precision = 0;
		info.name = sql[1];
		ColType   = sql[2];
		if(ColType =="integer")
			info.type = INT_V;
		else
		if(ColType =="real")
			info.type = DOUBLE_V;
		else
		if (ColType =="date")
			info.type = DATE_V;
		else
		if (ColType == "datetime")
			info.type = TIME_V;
		else
			info.type = STRING_V;
		out.Add(info);
	}
	return out;
}


//////////////////////////////////////////////////////////////////////

const char *Sqlite3ReadString(const char *s, String& stmt) {
	stmt.Cat(*s);
	int c = *s++;
	for(;;) {
		if(*s == '\0') break;
		else
		if(*s == '\'' && s[1] == '\'') {
			stmt.Cat('\'');
			s += 2;
		}
		else
		if(*s == c) {
			stmt.Cat(c);
			s++;
			break;
		}
		else
		if(*s == '\\') {
			stmt.Cat('\\');
			if(*++s)
				stmt.Cat(*s++);
		}
		else
			stmt.Cat(*s++);
	}
	return s;
}

bool Sqlite3PerformScript(const String& txt, StatementExecutor& se, Gate2<int, int> progress_canceled) {
	const char *text = txt;
	for(;;) {
		String stmt;
		while(*text <= 32 && *text > 0) text++;
		if(*text == '\0') break;
		for(;;) {
			if(*text == '\0')
				break;
			if(*text == ';')
				break;
			else
			if(*text == '\'')
				text = Sqlite3ReadString(text, stmt);
			else
			if(*text == '\"')
				text = Sqlite3ReadString(text, stmt);
			else
				stmt.Cat(*text++);
		}
		if(progress_canceled(text - txt.Begin(), txt.GetLength()))
			return false;
		if(!se.Execute(stmt))
			return false;
		if(*text) text++;
	}
	return true;
}

END_UPP_NAMESPACE

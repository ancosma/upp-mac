topic "TextCtrl";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[{_} 
[ {{10000@3 [s0; [*@(229)4 TextCtrl]]}}&]
[s3; &]
[s1;:TextCtrl`:`:class: [@(0.0.255)3 class][3 _][*3 TextCtrl][3 _:_][@(0.0.255)3 public][3 _][*@3;3 C
trl][3 , ][@(0.0.255)3 protected][3 _][*@3;3 TextArrayOps]&]
[s9;%% A base class that implements operations common for [^`:`:DocEdit^ DocEdit] 
(raw text editor where lines `- LF delimited sections `- are 
displayed as paragraphs `- wrapped when they do not fit the width) 
and [^`:`:LineEdit^ LineEdit].(where lines are simply lines and 
are never wrapped). TextCtrl works in UNICODE, but is able performing 
conversions to any of predefined charsets. The Value of TextCtrl 
is contained text; setting it is the same as using Set method, 
reading is equivalent to Get method.&]
[s3;%% &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Protected Method List]]}}&]
[s3; &]
[s5;:TextCtrl`:`:SelectionChanged`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* SelectionC
hanged]()&]
[s2;%% Selection anchor or cursor has changed. (called even when 
there is no selection, just cursor). If you override this, you 
should call base`-class version in the body.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:ClearLines`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* ClearLines]()&]
[s2;%% The content was removed (e.g. Clear or Load operations). If 
you override this, you should call base`-class version in the 
body.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:InsertLines`(int`,int`): [@(0.0.255) virtual] [@(0.0.255) void]_[* InsertL
ines]([@(0.0.255) int]_[*@3 line], [@(0.0.255) int]_[*@3 count])&]
[s2;%% [%-*@3 count] lines at [%-*@3 line] position were inserted. If 
you override this, you should call base`-class version in the 
body.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:RemoveLines`(int`,int`): [@(0.0.255) virtual] [@(0.0.255) void]_[* RemoveL
ines]([@(0.0.255) int]_[*@3 line], [@(0.0.255) int]_[*@3 count])&]
[s2;%% [%-*@3 count] lines at [%-*@3 line] position were removed. If 
you override this, you should call base`-class version in the 
body.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:PreInsert`(int`,const WString`&`): [@(0.0.255) virtual] 
[@(0.0.255) void]_[* PreInsert]([@(0.0.255) int]_[*@3 pos], [@(0.0.255) const]_[_^WString^ WS
tring][@(0.0.255) `&]_[*@3 text])&]
[s2;%% Called before any [%-*@3 text] gets inserted at [%-*@3 pos.] If 
you override this, you should call base`-class version in the 
body.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:PostInsert`(int`,const WString`&`): [@(0.0.255) virtual] 
[@(0.0.255) void]_[* PostInsert]([@(0.0.255) int]_[*@3 pos], [@(0.0.255) const]_[_^WString^ W
String][@(0.0.255) `&]_[*@3 text])&]
[s2;%% Called after [%-*@3 text] was inserted at [%-*@3 pos.] If you 
override this, you should call base`-class version in the body.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:PreRemove`(int`,int`): [@(0.0.255) virtual] [@(0.0.255) void]_[* PreRemove
]([@(0.0.255) int]_[*@3 pos], [@(0.0.255) int]_[*@3 size])&]
[s2;%% Called before [%-*@3 size] characters get removed from the [%-*@3 pos]. 
If you override this, you should call base`-class version in 
the body.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:PostRemove`(int`,int`): [@(0.0.255) virtual] [@(0.0.255) void]_[* PostRemo
ve]([@(0.0.255) int]_[*@3 pos], [@(0.0.255) int]_[*@3 size])&]
[s2;%% Called after [%-*@3 size] characters was removed from the [%-*@3 pos]. 
 If you override this, you should call base`-class version in 
the body.&]
[s3;%% &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:TextCtrl`:`:RefreshLine`(int`): [@(0.0.255) virtual] [@(0.0.255) void]_[* RefreshLine](
[@(0.0.255) int]_[*@3 i])&]
[s2;%% This virtual method is called by TextCtrl when the content 
of line [%-*@3 i] is changed. If you override this, you should 
call base`-class version in the body.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:WhenBar: [_^Callback1^ Callback1][@(0.0.255) <]Bar[@(0.0.255) `&>]_[* WhenBa
r]&]
[s2;%% This callback is invoked when user right`-clicks the widget 
and should specify local menu. It is initialized by the constructor 
to the StdBar method.  &]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:WhenState: [_^Callback^ Callback]_[* WhenState]&]
[s2;%% This callback is invoked when the status of widget changes 
from not`-modified to modified (`"dirty`") or back.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:CachePos`(int`): [@(0.0.255) void]_[* CachePos]([@(0.0.255) int]_[*@3 pos])&]
[s2;%% This is specific optimization hint to the widget saying that 
following operations will be performed near after [%-*@3 pos]. 
Unlikely to be used in the client code.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:Load`(Stream`&`,byte`): [@(0.0.255) void]_[* Load]([_^Stream^ Stream][@(0.0.255) `&
]_[*@3 s], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2;%% Loads the text from the stream with defined [%-*@3 charset].&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:Save`(Stream`&`,byte`,bool`)const: [@(0.0.255) void]_[* Save]([_^Stream^ S
tream][@(0.0.255) `&]_[*@3 s], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT, 
bool [*@3 crlf])_[@(0.0.255) const]&]
[s2;%% Saves the text to the stream with defined [%-*@3 charset]. Characters 
that cannot be represented in suggested [%-*@3 charset] are saved 
as `'?`'. If [%-*@3 crlf] is true, line endings are forced to be 
`"`\r`\n`" even on POSIX platforms.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetInvalidCharPos`(byte`)const: [@(0.0.255) int]_[* GetInvalidCharPos]([_^byte^ b
yte]_[*@3 charset]_`=_CHARSET`_DEFAULT)_[@(0.0.255) const]&]
[s2;%% Returns the position of the first character that cannot be 
represented [%-*@3 charset]. If there is none, returns negative 
value.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:CheckCharset`(byte`)const: [@(0.0.255) bool]_[* CheckCharset]([_^byte^ byt
e]_[*@3 charset]_`=_CHARSET`_DEFAULT)_[@(0.0.255) const]&]
[s2; [%% Same as GetInvalidCharPos(]charset) < 0.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:Set`(const WString`&`): [@(0.0.255) void]_[* Set]([@(0.0.255) const]_[_^WString^ W
String][@(0.0.255) `&]_[*@3 s])&]
[s5;:TextCtrl`:`:Set`(const String`&`,byte`): [@(0.0.255) void]_[* Set]([@(0.0.255) const]_
[_^String^ String][@(0.0.255) `&]_[*@3 s], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT
)&]
[s2;%% Sets the text in the widget.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:Get`(byte`)const: [_^String^ String]_[* Get]([_^byte^ byte]_[*@3 charset]_`=
_CHARSET`_DEFAULT)_[@(0.0.255) const]&]
[s2;%% Gets the text in the widget.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:Get`(int`,int`,byte`)const: [_^String^ String]_[* Get]([@(0.0.255) int]_[*@3 p
os], [@(0.0.255) int]_[*@3 size], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)_[@(0.0.255) c
onst]&]
[s2;%% Gets the part of text in the widget.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetW`(int`,int`)const: [_^WString^ WString]_[* GetW]([@(0.0.255) int]_[*@3 p
os], [@(0.0.255) int]_[*@3 size])_[@(0.0.255) const]&]
[s2;%% Gets the part of text in the widget in UNICODE.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetW`(`)const: [_^WString^ WString]_[* GetW]()_[@(0.0.255) const]&]
[s2;%% Gets the text in the widget in UNICODE.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:ClearDirty`(`): [@(0.0.255) void]_[* ClearDirty]()&]
[s2;%% Clears dirty flag. Note that the difference between `"dirty`" 
flag and `"modified`" flag of Ctrl interface is that `"dirty`" 
flag can get cleared by undo operation, when the text is restored 
to the original state (one after setting it or at ClearDirty).&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:IsDirty`(`)const: [@(0.0.255) bool]_[* IsDirty]()_[@(0.0.255) const]&]
[s2;%% Tests if text is different from `"original`" state (one after 
setting it or at ClearDirty).&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%% Empties the text.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetLinePos`(int`&`)const: [@(0.0.255) int]_[* GetLinePos]([@(0.0.255) int`&
]_[*@3 pos])_[@(0.0.255) const]&]
[s2;%% Returns the line where character at offset [%-*@3 pos] resides; 
[%-*@3 pos] is altered to contain the position withing the line.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetPos`(int`,int`)const: [@(0.0.255) int]_[* GetPos]([@(0.0.255) int]_[*@3 l
ine], [@(0.0.255) int]_[*@3 column])_[@(0.0.255) const]&]
[s2;%% Returns the offset of character at [%-*@3 line] and [%-*@3 column] 
index within the line.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetPos`(int`)const: [@(0.0.255) int]_[* GetPos]([@(0.0.255) int]_[*@3 line])
_[@(0.0.255) const]&]
[s2;%% Same as GetPos([%-*@3 line], 0).&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetLine`(int`)const: [@(0.0.255) int]_[* GetLine]([@(0.0.255) int]_[*@3 pos])
_[@(0.0.255) const]&]
[s2;%% Similar to GetLinePos, but does not alter [%-*@3 pos] parameter.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetUtf8Line`(int`)const: [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[* GetUtf8Line]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2;%% Returns the line [%-*@3 i] in UTF`-8 encoding.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetWLine`(int`)const: [_^WString^ WString]_[* GetWLine]([@(0.0.255) int]_[*@3 i
])_[@(0.0.255) const]&]
[s2;%% Returns the line [%-*@3 i] in UNICODE.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetEncodedLine`(int`,byte`)const: [_^String^ String]_[* GetEncodedLine](
[@(0.0.255) int]_[*@3 i], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)_[@(0.0.255) con
st]&]
[s2;%% Returns the line [%-*@3 i] required encoding.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetLineLength`(int`)const: [@(0.0.255) int]_[* GetLineLength]([@(0.0.255) i
nt]_[*@3 i])_[@(0.0.255) const]&]
[s2;%% Returns the length of line [%-*@3 i].&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetLineCount`(`)const: [@(0.0.255) int]_[* GetLineCount]()_[@(0.0.255) con
st]&]
[s2;%% Returns the number of lines.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetChar`(int`)const: [@(0.0.255) virtual] [@(0.0.255) int]_[* GetChar]([@(0.0.255) i
nt]_[*@3 pos])_[@(0.0.255) const]&]
[s5;:TextCtrl`:`:operator`[`]`(int`)const: [@(0.0.255) int]_[* operator`[`]]([@(0.0.255) in
t]_[*@3 pos])_[@(0.0.255) const]&]
[s2;%% Returns the UNICODE character at [%-*@3 pos] offset.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetLength`(`)const: [@(0.0.255) virtual] [@(0.0.255) int]_[* GetLength]()_
[@(0.0.255) const]&]
[s2;%% Returns the total number of characters in the text.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetCursor`(`)const: [@(0.0.255) int]_[* GetCursor]()_[@(0.0.255) const]&]
[s2;%% Returns the position of cursor.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetCursorLine`(`): [@(0.0.255) int]_[* GetCursorLine]()&]
[s2;%% Same as GetLine(GetCursor).&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:SetSelection`(int`,int`): [@(0.0.255) void]_[* SetSelection]([@(0.0.255) i
nt]_[*@3 anchor]_`=_[@3 0], [@(0.0.255) int]_[*@3 cursor]_`=_INT`_MAX)&]
[s2;%% Sets the selection. If [%-*@3 anchor] or [%-*@3 cursor] are out 
of range, they are `"fixed`". If they are equal, method changes 
the position of cursor.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:IsSelection`(`)const: [@(0.0.255) bool]_[* IsSelection]()_[@(0.0.255) cons
t]&]
[s2;%% Tests whether there is non`-empty selection.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetSelection`(int`&`,int`&`)const: [@(0.0.255) bool]_[* GetSelection]([@(0.0.255) i
nt`&]_[*@3 l], [@(0.0.255) int`&]_[*@3 h])_[@(0.0.255) const]&]
[s2;%% Returns the selection lower and upper bounds.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetSelection`(byte`)const: [_^String^ String]_[* GetSelection]([_^byte^ by
te]_[*@3 charset]_`=_CHARSET`_DEFAULT)_[@(0.0.255) const]&]
[s2;%% Returns the selected text in given [%-*@3 charset].&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetSelectionW`(`)const: [_^WString^ WString]_[* GetSelectionW]()_[@(0.0.255) c
onst]&]
[s2;%% Returns the selected text in UNICODE.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:ClearSelection`(`): [@(0.0.255) void]_[* ClearSelection]()&]
[s2;%% Cancels the selection.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:RemoveSelection`(`): [@(0.0.255) bool]_[* RemoveSelection]()&]
[s2;%% Deletes the selection text.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:SetCursor`(int`): [@(0.0.255) void]_[* SetCursor]([@(0.0.255) int]_[*@3 curs
or])&]
[s2;%% Places cursor at new position.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:Paste`(const WString`&`): [@(0.0.255) int]_[* Paste]([@(0.0.255) const]_[_^WString^ W
String][@(0.0.255) `&]_[*@3 text])&]
[s2;%% Pastes [%-*@3 text] at current cursor position.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:Insert`(int`,const WString`&`): [@(0.0.255) int]_[* Insert]([@(0.0.255) in
t]_[*@3 pos], [@(0.0.255) const]_[_^WString^ WString][@(0.0.255) `&]_[*@3 txt])&]
[s2;%% Inserts [%-*@3 txt] at [%-*@3 pos].&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:Insert`(int`,const String`&`,byte`): [@(0.0.255) int]_[* Insert]([@(0.0.255) i
nt]_[*@3 pos], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 txt], 
[_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2;%% Inserts [%-*@3 txt] encoded with [%-*@3 charset ]at [%-*@3 pos].&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:Insert`(int`,const char`*`): [@(0.0.255) int]_[* Insert]([@(0.0.255) int]_
[*@3 pos], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 txt])&]
[s2;%% Inserts [%-*@3 txt] at [%-*@3 pos] using default charset.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:Remove`(int`,int`): [@(0.0.255) void]_[* Remove]([@(0.0.255) int]_[*@3 pos],
 [@(0.0.255) int]_[*@3 size])&]
[s2;%% Removes the text.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:NextUndo`(`): [@(0.0.255) void]_[* NextUndo]()&]
[s2;%% Marks the beginning of next undo block. All text altering 
operations (inserts and removes) are recorded into undo buffer 
in sections where each sections represents single user action. 
NextUndo tells TextCtrl that the next altering operation belongs 
to the new section. Note that NextUndo does not create a new 
section (yet); calling it twice without any altering operation 
in between is the same as calling it once.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:Undo`(`): [@(0.0.255) void]_[* Undo]()&]
[s2;%% Performs undo.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:Redo`(`): [@(0.0.255) void]_[* Redo]()&]
[s2;%% Performs redo.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:IsUndo`(`)const: [@(0.0.255) bool]_[* IsUndo]()_[@(0.0.255) const]&]
[s2;%% Tests whether there are some undo records.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:IsRedo`(`)const: [@(0.0.255) bool]_[* IsRedo]()_[@(0.0.255) const]&]
[s2;%% Tests whether there are some redo records.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:ClearUndo`(`): [@(0.0.255) void]_[* ClearUndo]()&]
[s2;%% Clears all undo and redo records.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:PickUndoData`(`): [_^TextCtrl`:`:UndoData^ UndoData]_[* PickUndoData]()&]
[s2;%% Picks undo and redo record. This is useful when single widget 
is used to edit various text, this allows to change undo/redo 
records with the text. Must be followed by either SetPickUndoData 
or ClearUndo before performing any text altering operation.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:SetPickUndoData`(pick`_ TextCtrl`:`:UndoData`&`): [@(0.0.255) void]_[* S
etPickUndoData]([@(0.128.128) pick`_]_[_^TextCtrl`:`:UndoData^ UndoData][@(0.0.255) `&]_
[*@3 data])&]
[s2;%% Sets the undo/redo records. [%-@3 data] is picked (destroyed) 
during the operation.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:Cut`(`): [@(0.0.255) void]_[* Cut]()&]
[s2;%% Performs Cut.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:Copy`(`): [@(0.0.255) void]_[* Copy]()&]
[s2;%% Performs Copy.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:Paste`(`): [@(0.0.255) void]_[* Paste]()&]
[s2;%% Performs Paste.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:SelectAll`(`): [@(0.0.255) void]_[* SelectAll]()&]
[s2;%% Selects the whole text.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:StdBar`(Bar`&`): [@(0.0.255) void]_[* StdBar]([_^Bar^ Bar][@(0.0.255) `&]_[*@3 m
enu])&]
[s2;%% Provides the standard local [%-*@3 menu].&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:SetCharset`(byte`): [@(0.0.255) void]_[* SetCharset]([_^byte^ byte]_[*@3 cs])
&]
[s2;%% Sets the input character encoding of widget. This character 
set is used to decide which characters can be entered by the 
user into the text.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetCharset`(`)const: [_^byte^ byte]_[* GetCharset]()_[@(0.0.255) const]&]
[s2;%% Returns the value set by SetCharset.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:SetColor`(int`,Color`): [@(0.0.255) void]_[* SetColor]([@(0.0.255) int]_[*@3 i
], [_^Color^ Color]_[*@3 c])&]
[s2;%% Sets one of colors used to display the text.&]
[s3;%% &]
[ {{4443:5557<288;>864; [s0; [+75 TextCtrl`::][*+75 INK`_NORMAL]]
:: [s0;%% [+75 Normal text color.]]
:: [s0; [+75 TextCtrl`::][*+75 INK`_DISABLED]]
:: [s0;%% [+75 Text color when widget is disabled. ]]
:: [s0; [+75 TextCtrl`::][*+75 INK`_SELECTED]]
:: [s0;%% [+75 Selected text color.]]
:: [s0; [+75 TextCtrl`::][*+75 PAPER`_NORMAL]]
:: [s0;%% [+75 Normal paper color.]]
:: [s0; [+75 TextCtrl`::][*+75 PAPER`_READONLY]]
:: [s0;%% [+75 Paper color when widget is read`-only.]]
:: [s0; [+75 TextCtrl`::][*+75 PAPER`_SELECTED]]
:: [s0;%% [+75 Selected paper color.]]
:: [s0; [+75 TextCtrl`::][*+75 COLOR`_COUNT]]
:: [s0;%% [+75 Total number of color indexes.]]}}&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetColor`(int`)const: [_^Color^ Color]_[* GetColor]([@(0.0.255) int]_[*@3 i])
_[@(0.0.255) const]&]
[s2;%% Gets the color used to display the text.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:UndoSteps`(int`): [_^TextCtrl^ TextCtrl][@(0.0.255) `&]_[* UndoSteps]([@(0.0.255) i
nt]_[*@3 n])&]
[s2;%% Sets the maximum number of undo steps.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:ProcessTab`(bool`): [_^TextCtrl^ TextCtrl][@(0.0.255) `&]_[* ProcessTab]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% If active (default), widget accepts K`_TAB keystrokes (inserts 
`'`\t`').&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:NoProcessTab`(`): [_^TextCtrl^ TextCtrl][@(0.0.255) `&]_[* NoProcessTab]()
&]
[s2;%% Same as ProcessTab(false).&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:NoBackground`(bool`): [_^TextCtrl^ TextCtrl][@(0.0.255) `&]_[* NoBackgroun
d]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% Sets the widget into transparent mode `- background is not 
painted and Transparent is activated `- a result, anything painted 
behind the widget is visible, allowing client code to provide 
any background it needs.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:IsProcessTab`(`): [@(0.0.255) bool]_[* IsProcessTab]()&]
[s2;%% Tests whether ProcessTab mode is active.&]
[s3;%% &]
[s0;%% &]
[s0;%% &]
[s0;%% &]
[ {{10000@3 [s0; [*@(229)4 TextCtrl`::UndoData]]}}&]
[s3; &]
[s1;:TextCtrl`:`:UndoData`:`:struct: [@(0.0.255)3 struct][3 _][*3 UndoData]&]
[s9;%% This structure contains informations about undo/redo state 
of the widget. It can be with PickUndoData and SetPickUndoDate 
to store and restore undo/redo state.&]
[s3;%% &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:TextCtrl`:`:UndoData`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%% Clears the undo/redo state&]
[s3;%% &]
[s3;%% .]
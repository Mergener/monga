source_filename = "tests/codegen_cases/null_array_elements.mon"

%string = type { i32, i32 }

declare %string* @RtInternal_StrFromSZ(i8*)
declare i8* @RtInternal_GcAlloc(i32)
declare void @RtInternal_Init()

define void @main() {
	call void @RtInternal_Init()
	%t.0 = alloca i8**

	store i8** null, i8*** %t.0
	%t.1 = mul i32 32, 8
	%t.2 = call i8* @RtInternal_GcAlloc(i32 %t.1)
	%t.3 = bitcast i8* %t.2 to i8**
	store i8** %t.3, i8*** %t.0

	%t.4 = sext i32 0 to i64
	%t.5 = load i8**, i8*** %t.0
	%t.6 = getelementptr i8*, i8** %t.5, i64 %t.4
	%t.7 = load i8*, i8** %t.6
	%t.8 = icmp eq i8* %t.7, null
	br i1 %t.8, label %l.0, label %l.1
l.0:
	%t.9 = call %string* @RtInternal_StrFromSZ(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.0, i32 0, i32 0))
	call void @printString(%string* %t.9)

	br label %l.2
l.1:
	%t.10 = call %string* @RtInternal_StrFromSZ(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0))
	call void @printString(%string* %t.10)

	br label %l.2
l.2:
	ret void
}

declare void @printString(%string*)

@str.0 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"False\00", align 1


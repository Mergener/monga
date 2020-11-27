source_filename = "tests/codegen_cases/null_array_elements.mon"

%string = type { i32, i32 }

declare %string* @RtInternal_StrFromSZ(i8*)
declare i8* @RtInternal_GcAlloc(i64)
declare void @RtInternal_EchoArray(i8*)
declare void @RtInternal_EchoObject(i8*)
declare void @RtInternal_EchoInteger(i64)
declare void @RtInternal_EchoReal(double)
declare void @RtInternal_EchoChar(i8)
declare void @RtInternal_EchoString(%string*)
declare i8* @RtInternal_GetAllocSize(i64)
declare void @RtInternal_Init()

define void @main() {
	call void @RtInternal_Init()
	%t.0 = alloca i8**

	store i8** null, i8*** %t.0
	%t.1 = sext i32 32 to i64
	%t.2 = mul i64 %t.1, 8
	%t.3 = call i8* @RtInternal_GcAlloc(i64 %t.2)
	%t.4 = bitcast i8* %t.3 to i8**
	store i8** %t.4, i8*** %t.0

	%t.5 = sext i32 0 to i64
	%t.6 = load i8**, i8*** %t.0
	%t.7 = getelementptr i8*, i8** %t.6, i64 %t.5
	%t.8 = load i8*, i8** %t.7
	%t.9 = icmp eq i8* %t.8, null
	br i1 %t.9, label %l.0, label %l.1
l.0:
	%t.10 = call %string* @RtInternal_StrFromSZ(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.0, i32 0, i32 0))
	call void @printString(%string* %t.10)

	br label %l.2
l.1:
	%t.11 = call %string* @RtInternal_StrFromSZ(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0))
	call void @printString(%string* %t.11)

	br label %l.2
l.2:
	ret void
}

declare void @printString(%string*)

@str.0 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"False\00", align 1


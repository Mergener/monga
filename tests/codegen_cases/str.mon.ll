source_filename = "tests/codegen_cases/str.mon"

%string = type { i32, i32 }

declare %string* @RtInternal_StrFromSZ(i8*)
declare i8* @RtInternal_GcAlloc(i32)
declare void @RtInternal_Init()

define void @main() {
	call void @RtInternal_Init()
	%t.0 = alloca %string*

	store %string* null, %string** %t.0
	%t.1 = call %string* @RtInternal_StrFromSZ(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.0, i32 0, i32 0))
	store %string* %t.1, %string** %t.0

	%t.2 = load %string*, %string** %t.0
	call void @printString(%string* %t.2)

	ret void
}

declare void @printString(%string*)

@str.0 = private unnamed_addr constant [5 x i8] c"test\00", align 1


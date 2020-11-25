source_filename = "tests/codegen_cases/globalvar.mon"

%string = type { i32, i32 }

declare %string* @RtInternal_StrFromSZ(i8*)
declare i8* @RtInternal_GcAlloc(i32)
declare void @RtInternal_Init()

@i = internal global i32 0

define void @foo() {
	%t.0 = load i32, i32* @i
	call void @printInteger(i32 %t.0)

	ret void
}

define void @main() {
	call void @RtInternal_Init()
	%t.0 = load i32, i32* @i
	call void @printInteger(i32 %t.0)

	store i32 5, i32* @i

	call void @foo()

	store i32 3, i32* @i

	call void @foo()

	ret void
}

declare void @printInteger(i32)



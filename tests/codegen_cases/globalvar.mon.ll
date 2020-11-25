source_filename = "tests/codegen_cases/globalvar.mon"

declare i8* @RtInternal_GcAlloc(i32)

@i = internal global i32 undef

define void @foo() {
	%t.0 = load i32, i32* @i
	call void @printInteger(i32 %t.0)

	ret void
}

define void @main() {
	store i32 5, i32* @i

	call void @foo()

	store i32 3, i32* @i

	call void @foo()

	ret void
}

declare void @printInteger(i32)

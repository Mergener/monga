source_filename = "tests/codegen_cases/10.mon"

declare i8* @RtInternal_GcAlloc(i32)

define void @main() {
	call void @printInteger(i32 10)

	ret void
}

declare void @printInteger(i32)

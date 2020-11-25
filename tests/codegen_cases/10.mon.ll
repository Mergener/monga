source_filename = "tests/codegen_cases/10.mon"

%string = type { i32, i32 }

declare %string* @RtInternal_StrFromSZ(i8*)
declare i8* @RtInternal_GcAlloc(i32)
declare void @RtInternal_Init()

define void @main() {
	call void @RtInternal_Init()
	call void @printInteger(i32 10)

	ret void
}

declare void @printInteger(i32)



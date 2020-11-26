source_filename = "tests/codegen_cases/char.mon"

%string = type { i32, i32 }

declare %string* @RtInternal_StrFromSZ(i8*)
declare i8* @RtInternal_GcAlloc(i32)
declare void @RtInternal_Init()

define void @main() {
	call void @RtInternal_Init()
	%t.0 = alloca i8

	store i8 0, i8* %t.0
	store i8 10, i8* %t.0

	ret void
}




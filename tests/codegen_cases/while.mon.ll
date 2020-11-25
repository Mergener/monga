source_filename = "tests/codegen_cases/while.mon"

%string = type { i32, i32 }

declare %string* @RtInternal_StrFromSZ(i8*)
declare i8* @RtInternal_GcAlloc(i32)
declare void @RtInternal_Init()

define void @main() {
	call void @RtInternal_Init()
	%t.0 = alloca i32

	store i32 0, i32* %t.0
	store i32 10, i32* %t.0

	%t.1 = load i32, i32* %t.0
	%t.2 = icmp sgt i32 %t.1, 0
	br i1 %t.2, label %l.1, label %l.0
l.1:
	%t.3 = load i32, i32* %t.0
	call void @printInteger(i32 %t.3)

	%t.4 = load i32, i32* %t.0
	%t.5 = sub i32 %t.4, 1
	store i32 %t.5, i32* %t.0

	br label %l.2
l.2:
	%t.6 = load i32, i32* %t.0
	%t.7 = icmp sgt i32 %t.6, 0
	br i1 %t.7, label %l.1, label %l.0
l.0:
	ret void
}

declare void @printInteger(i32)



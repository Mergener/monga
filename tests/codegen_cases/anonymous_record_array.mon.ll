source_filename = "tests/codegen_cases/anonymous_record_array.mon"

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

%.record_0 = type { i32, i32 }

define void @main() {
	call void @RtInternal_Init()
	%t.0 = alloca %.record_0**

	store %.record_0** null, %.record_0*** %t.0
	ret void
}




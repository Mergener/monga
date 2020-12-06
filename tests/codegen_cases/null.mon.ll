source_filename = "tests/codegen_cases/null.mon"

%string = type { i64, i64 }
%.array = type { i64 }

declare %string* @RtInternal_StrFromSZ(i8*)
declare i8* @RtInternal_GcAlloc(i64)
declare %.array* @RtInternal_GcAllocArray(i64, i64)
declare i64 @RtInternal_GetGcArrayElemCount(%.array*)
declare void @RtInternal_EchoArray(i8*)
declare void @RtInternal_EchoObject(i8*)
declare void @RtInternal_EchoInteger(i64)
declare void @RtInternal_EchoReal(double)
declare void @RtInternal_EchoChar(i8)
declare void @RtInternal_EchoString(%string*)
declare void @RtInternal_Hash(i8*, i32)
declare void @RtInternal_Init()

define void @main() {
	call void @RtInternal_Init()

	%t.0 = alloca %.array*
	store %.array* null, %.array** %t.0

	store %.array* null, %.array** %t.0

	%t.1 = load %.array*, %.array** %t.0
	%t.2 = icmp eq %.array* %t.1, null
	br i1 %t.2, label %l.0, label %l.1
l.0:
	%t.3 = sext i32 1 to i64
	call void @RtInternal_EchoInteger(i64 %t.3)

	br label %l.2
l.1:
	%t.4 = sext i32 0 to i64
	call void @RtInternal_EchoInteger(i64 %t.4)

	br label %l.2
l.2:
	ret void
}




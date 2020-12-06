source_filename = "tests/codegen_cases/null_array_elements.mon"

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

	%t.1 = sext i32 32 to i64
	%t.2 = call %.array* @RtInternal_GcAllocArray(i64 8, i64 %t.1)
	store %.array* %t.2, %.array** %t.0

	%t.3 = sext i32 0 to i64
	%t.4 = load %.array*, %.array** %t.0
	%t.5 = getelementptr %.array, %.array* %t.4, i64 1
	%t.6 = bitcast %.array* %t.5 to %.array**
	%t.7 = getelementptr %.array*, %.array** %t.6, i64 %t.3
	%t.8 = load %.array*, %.array** %t.7
	%t.9 = icmp eq %.array* %t.8, null
	br i1 %t.9, label %l.0, label %l.1
l.0:
	call void @RtInternal_EchoString(%string* bitcast ({ i64, i64, [5 x i8] }* @str.0 to %string*))

	br label %l.2
l.1:
	call void @RtInternal_EchoString(%string* bitcast ({ i64, i64, [6 x i8] }* @str.1 to %string*))

	br label %l.2
l.2:
	ret void
}


@str.0 = private constant { i64, i64, [5 x i8] } { i64 4, i64 5052420134498106531, [5 x i8] c"True\00" }
@str.1 = private constant { i64, i64, [6 x i8] } { i64 5, i64 881876403870646111, [6 x i8] c"False\00" }


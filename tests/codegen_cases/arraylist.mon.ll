source_filename = "tests/codegen_cases/arraylist.mon"

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

%TArrayList = type { %.array*, i32, i32 }

define %TArrayList* @Al_Create() {
	%t.0 = alloca %TArrayList*
	store %TArrayList* null, %TArrayList** %t.0

	%t.1 = call i8* @RtInternal_GcAlloc(i64 16)
	%t.2 = bitcast i8* %t.1 to %TArrayList*
	store %TArrayList* %t.2, %TArrayList** %t.0

	%t.3 = load %TArrayList*, %TArrayList** %t.0
	%t.4 = getelementptr inbounds %TArrayList, %TArrayList* %t.3, i32 0, i32 1
	store i32 0, i32* %t.4

	%t.5 = load %TArrayList*, %TArrayList** %t.0
	%t.6 = getelementptr inbounds %TArrayList, %TArrayList* %t.5, i32 0, i32 0
	%t.7 = sext i32 8 to i64
	%t.8 = call %.array* @RtInternal_GcAllocArray(i64 4, i64 %t.7)
	store %.array* %t.8, %.array** %t.6

	%t.9 = load %TArrayList*, %TArrayList** %t.0
	%t.10 = getelementptr inbounds %TArrayList, %TArrayList* %t.9, i32 0, i32 2
	store i32 8, i32* %t.10

	%t.11 = load %TArrayList*, %TArrayList** %t.0
	ret %TArrayList* %t.11
}

define void @Al_Resize(%TArrayList*, i32) {
	%t.0 = alloca %TArrayList*
	store %TArrayList* %0, %TArrayList** %t.0
	%t.1 = alloca i32
	store i32 %1, i32* %t.1

	%t.2 = alloca %.array*
	store %.array* null, %.array** %t.2

	%t.3 = alloca i32
	store i32 0, i32* %t.3

	%t.4 = alloca i32
	store i32 0, i32* %t.4

	%t.5 = load i32, i32* %t.1
	%t.6 = sext i32 %t.5 to i64
	%t.7 = call %.array* @RtInternal_GcAllocArray(i64 4, i64 %t.6)
	store %.array* %t.7, %.array** %t.2

	%t.8 = load i32, i32* %t.1
	%t.9 = load %TArrayList*, %TArrayList** %t.0
	%t.10 = getelementptr inbounds %TArrayList, %TArrayList* %t.9, i32 0, i32 1
	%t.11 = load i32, i32* %t.10
	%t.12 = icmp slt i32 %t.8, %t.11
	br i1 %t.12, label %l.0, label %l.1
l.0:
	%t.13 = load i32, i32* %t.1
	br label %l.3
l.3:
	br label %l.2
l.1:
	%t.14 = load %TArrayList*, %TArrayList** %t.0
	%t.15 = getelementptr inbounds %TArrayList, %TArrayList* %t.14, i32 0, i32 1
	%t.16 = load i32, i32* %t.15
	br label %l.4
l.4:
	br label %l.2
l.2:
	%t.17 = phi i32 [%t.13, %l.3], [%t.16, %l.4]
	store i32 %t.17, i32* %t.4

	store i32 0, i32* %t.3

	%t.18 = load i32, i32* %t.3
	%t.19 = load i32, i32* %t.4
	%t.20 = icmp slt i32 %t.18, %t.19
	br i1 %t.20, label %l.6, label %l.5
l.6:
	%t.21 = load i32, i32* %t.3
	%t.22 = sext i32 %t.21 to i64
	%t.23 = load %.array*, %.array** %t.2
	%t.24 = getelementptr %.array, %.array* %t.23, i64 1
	%t.25 = bitcast %.array* %t.24 to i32*
	%t.26 = getelementptr i32, i32* %t.25, i64 %t.22
	%t.27 = load i32, i32* %t.3
	%t.28 = sext i32 %t.27 to i64
	%t.29 = load %TArrayList*, %TArrayList** %t.0
	%t.30 = getelementptr inbounds %TArrayList, %TArrayList* %t.29, i32 0, i32 0
	%t.31 = load %.array*, %.array** %t.30
	%t.32 = getelementptr %.array, %.array* %t.31, i64 1
	%t.33 = bitcast %.array* %t.32 to i32*
	%t.34 = getelementptr i32, i32* %t.33, i64 %t.28
	%t.35 = load i32, i32* %t.34
	store i32 %t.35, i32* %t.26

	%t.36 = load i32, i32* %t.3
	%t.37 = add i32 %t.36, 1
	store i32 %t.37, i32* %t.3

	br label %l.7
l.7:
	%t.38 = load i32, i32* %t.3
	%t.39 = load i32, i32* %t.4
	%t.40 = icmp slt i32 %t.38, %t.39
	br i1 %t.40, label %l.6, label %l.5
l.5:
	%t.41 = load %TArrayList*, %TArrayList** %t.0
	%t.42 = getelementptr inbounds %TArrayList, %TArrayList* %t.41, i32 0, i32 2
	%t.43 = load i32, i32* %t.1
	store i32 %t.43, i32* %t.42

	%t.44 = load %TArrayList*, %TArrayList** %t.0
	%t.45 = getelementptr inbounds %TArrayList, %TArrayList* %t.44, i32 0, i32 0
	%t.46 = load %.array*, %.array** %t.2
	store %.array* %t.46, %.array** %t.45

	ret void
}

define void @Al_Add(%TArrayList*, i32) {
	%t.0 = alloca %TArrayList*
	store %TArrayList* %0, %TArrayList** %t.0
	%t.1 = alloca i32
	store i32 %1, i32* %t.1

	%t.2 = load %TArrayList*, %TArrayList** %t.0
	%t.3 = getelementptr inbounds %TArrayList, %TArrayList* %t.2, i32 0, i32 1
	%t.4 = load i32, i32* %t.3
	%t.5 = load %TArrayList*, %TArrayList** %t.0
	%t.6 = getelementptr inbounds %TArrayList, %TArrayList* %t.5, i32 0, i32 2
	%t.7 = load i32, i32* %t.6
	%t.8 = icmp eq i32 %t.4, %t.7
	br i1 %t.8, label %l.0, label %l.1
l.0:
	%t.9 = load %TArrayList*, %TArrayList** %t.0
	%t.10 = load %TArrayList*, %TArrayList** %t.0
	%t.11 = getelementptr inbounds %TArrayList, %TArrayList* %t.10, i32 0, i32 2
	%t.12 = load i32, i32* %t.11
	%t.13 = mul i32 %t.12, 2
	call void @Al_Resize(%TArrayList* %t.9, i32 %t.13)

	br label %l.1
l.1:
	%t.14 = load %TArrayList*, %TArrayList** %t.0
	%t.15 = getelementptr inbounds %TArrayList, %TArrayList* %t.14, i32 0, i32 1
	%t.16 = load i32, i32* %t.15
	%t.17 = sext i32 %t.16 to i64
	%t.18 = load %TArrayList*, %TArrayList** %t.0
	%t.19 = getelementptr inbounds %TArrayList, %TArrayList* %t.18, i32 0, i32 0
	%t.20 = load %.array*, %.array** %t.19
	%t.21 = getelementptr %.array, %.array* %t.20, i64 1
	%t.22 = bitcast %.array* %t.21 to i32*
	%t.23 = getelementptr i32, i32* %t.22, i64 %t.17
	%t.24 = load i32, i32* %t.1
	store i32 %t.24, i32* %t.23

	%t.25 = load %TArrayList*, %TArrayList** %t.0
	%t.26 = getelementptr inbounds %TArrayList, %TArrayList* %t.25, i32 0, i32 1
	%t.27 = load %TArrayList*, %TArrayList** %t.0
	%t.28 = getelementptr inbounds %TArrayList, %TArrayList* %t.27, i32 0, i32 1
	%t.29 = load i32, i32* %t.28
	%t.30 = add i32 %t.29, 1
	store i32 %t.30, i32* %t.26

	ret void
}

define void @Al_RemoveIdx(%TArrayList*, i32) {
	%t.0 = alloca %TArrayList*
	store %TArrayList* %0, %TArrayList** %t.0
	%t.1 = alloca i32
	store i32 %1, i32* %t.1

	%t.2 = alloca i32
	store i32 0, i32* %t.2

	%t.3 = load i32, i32* %t.1
	%t.4 = icmp slt i32 %t.3, 0
	br i1 %t.4, label %l.0, label %l.2
l.2:
	%t.5 = load i32, i32* %t.1
	%t.6 = load %TArrayList*, %TArrayList** %t.0
	%t.7 = getelementptr inbounds %TArrayList, %TArrayList* %t.6, i32 0, i32 1
	%t.8 = load i32, i32* %t.7
	%t.9 = icmp sge i32 %t.5, %t.8
	br i1 %t.9, label %l.0, label %l.1
l.0:
	ret void
l.1:
	%t.10 = load %TArrayList*, %TArrayList** %t.0
	%t.11 = getelementptr inbounds %TArrayList, %TArrayList* %t.10, i32 0, i32 1
	%t.12 = load i32, i32* %t.11
	%t.13 = load %TArrayList*, %TArrayList** %t.0
	%t.14 = getelementptr inbounds %TArrayList, %TArrayList* %t.13, i32 0, i32 2
	%t.15 = load i32, i32* %t.14
	%t.16 = sdiv i32 %t.15, 3
	%t.17 = icmp slt i32 %t.12, %t.16
	br i1 %t.17, label %l.3, label %l.4
l.3:
	%t.18 = load %TArrayList*, %TArrayList** %t.0
	%t.19 = load %TArrayList*, %TArrayList** %t.0
	%t.20 = getelementptr inbounds %TArrayList, %TArrayList* %t.19, i32 0, i32 2
	%t.21 = load i32, i32* %t.20
	%t.22 = sdiv i32 %t.21, 2
	call void @Al_Resize(%TArrayList* %t.18, i32 %t.22)

	br label %l.4
l.4:
	%t.23 = load i32, i32* %t.1
	store i32 %t.23, i32* %t.2

	%t.24 = load %TArrayList*, %TArrayList** %t.0
	%t.25 = getelementptr inbounds %TArrayList, %TArrayList* %t.24, i32 0, i32 1
	%t.26 = load %TArrayList*, %TArrayList** %t.0
	%t.27 = getelementptr inbounds %TArrayList, %TArrayList* %t.26, i32 0, i32 1
	%t.28 = load i32, i32* %t.27
	%t.29 = sub i32 %t.28, 1
	store i32 %t.29, i32* %t.25

	%t.30 = load i32, i32* %t.2
	%t.31 = load %TArrayList*, %TArrayList** %t.0
	%t.32 = getelementptr inbounds %TArrayList, %TArrayList* %t.31, i32 0, i32 1
	%t.33 = load i32, i32* %t.32
	%t.34 = icmp slt i32 %t.30, %t.33
	br i1 %t.34, label %l.6, label %l.5
l.6:
	%t.35 = load i32, i32* %t.2
	%t.36 = sext i32 %t.35 to i64
	%t.37 = load %TArrayList*, %TArrayList** %t.0
	%t.38 = getelementptr inbounds %TArrayList, %TArrayList* %t.37, i32 0, i32 0
	%t.39 = load %.array*, %.array** %t.38
	%t.40 = getelementptr %.array, %.array* %t.39, i64 1
	%t.41 = bitcast %.array* %t.40 to i32*
	%t.42 = getelementptr i32, i32* %t.41, i64 %t.36
	%t.43 = load i32, i32* %t.2
	%t.44 = add i32 %t.43, 1
	%t.45 = sext i32 %t.44 to i64
	%t.46 = load %TArrayList*, %TArrayList** %t.0
	%t.47 = getelementptr inbounds %TArrayList, %TArrayList* %t.46, i32 0, i32 0
	%t.48 = load %.array*, %.array** %t.47
	%t.49 = getelementptr %.array, %.array* %t.48, i64 1
	%t.50 = bitcast %.array* %t.49 to i32*
	%t.51 = getelementptr i32, i32* %t.50, i64 %t.45
	%t.52 = load i32, i32* %t.51
	store i32 %t.52, i32* %t.42

	%t.53 = load i32, i32* %t.2
	%t.54 = add i32 %t.53, 1
	store i32 %t.54, i32* %t.2

	br label %l.7
l.7:
	%t.55 = load i32, i32* %t.2
	%t.56 = load %TArrayList*, %TArrayList** %t.0
	%t.57 = getelementptr inbounds %TArrayList, %TArrayList* %t.56, i32 0, i32 1
	%t.58 = load i32, i32* %t.57
	%t.59 = icmp slt i32 %t.55, %t.58
	br i1 %t.59, label %l.6, label %l.5
l.5:
	ret void
}

define i32 @Al_Get(%TArrayList*, i32) {
	%t.0 = alloca %TArrayList*
	store %TArrayList* %0, %TArrayList** %t.0
	%t.1 = alloca i32
	store i32 %1, i32* %t.1

	%t.2 = load i32, i32* %t.1
	%t.3 = icmp slt i32 %t.2, 0
	br i1 %t.3, label %l.0, label %l.2
l.2:
	%t.4 = load i32, i32* %t.1
	%t.5 = load %TArrayList*, %TArrayList** %t.0
	%t.6 = getelementptr inbounds %TArrayList, %TArrayList* %t.5, i32 0, i32 1
	%t.7 = load i32, i32* %t.6
	%t.8 = icmp sge i32 %t.4, %t.7
	br i1 %t.8, label %l.0, label %l.1
l.0:
	%t.10 = sub i32 0, 1
	ret i32 %t.10
l.1:
	%t.11 = load i32, i32* %t.1
	%t.12 = sext i32 %t.11 to i64
	%t.13 = load %TArrayList*, %TArrayList** %t.0
	%t.14 = getelementptr inbounds %TArrayList, %TArrayList* %t.13, i32 0, i32 0
	%t.15 = load %.array*, %.array** %t.14
	%t.16 = getelementptr %.array, %.array* %t.15, i64 1
	%t.17 = bitcast %.array* %t.16 to i32*
	%t.18 = getelementptr i32, i32* %t.17, i64 %t.12
	%t.19 = load i32, i32* %t.18
	ret i32 %t.19
}

define void @Al_Print(%TArrayList*) {
	%t.0 = alloca %TArrayList*
	store %TArrayList* %0, %TArrayList** %t.0

	%t.1 = alloca i32
	store i32 0, i32* %t.1

	%t.2 = load %TArrayList*, %TArrayList** %t.0
	%t.3 = getelementptr inbounds %TArrayList, %TArrayList* %t.2, i32 0, i32 1
	%t.4 = load i32, i32* %t.3
	store i32 %t.4, i32* %t.1

	%t.5 = alloca i32
	store i32 0, i32* %t.5

	store i32 0, i32* %t.5

	%t.6 = load i32, i32* %t.5
	%t.7 = load i32, i32* %t.1
	%t.8 = icmp slt i32 %t.6, %t.7
	br i1 %t.8, label %l.1, label %l.0
l.1:
	%t.9 = load %TArrayList*, %TArrayList** %t.0
	%t.10 = load i32, i32* %t.5
	%t.11 = call i32 @Al_Get(%TArrayList* %t.9, i32 %t.10)
	%t.12 = sext i32 %t.11 to i64
	call void @RtInternal_EchoInteger(i64 %t.12)

	%t.13 = load i32, i32* %t.5
	%t.14 = add i32 %t.13, 1
	store i32 %t.14, i32* %t.5

	br label %l.2
l.2:
	%t.15 = load i32, i32* %t.5
	%t.16 = load i32, i32* %t.1
	%t.17 = icmp slt i32 %t.15, %t.16
	br i1 %t.17, label %l.1, label %l.0
l.0:
	ret void
}

define void @main() {
	call void @RtInternal_Init()

	%t.0 = alloca %TArrayList*
	store %TArrayList* null, %TArrayList** %t.0

	%t.1 = call %TArrayList* @Al_Create()
	store %TArrayList* %t.1, %TArrayList** %t.0

	%t.2 = load %TArrayList*, %TArrayList** %t.0
	call void @Al_Add(%TArrayList* %t.2, i32 5)

	%t.3 = load %TArrayList*, %TArrayList** %t.0
	call void @Al_Add(%TArrayList* %t.3, i32 12)

	%t.4 = load %TArrayList*, %TArrayList** %t.0
	call void @Al_Add(%TArrayList* %t.4, i32 13)

	%t.5 = load %TArrayList*, %TArrayList** %t.0
	call void @Al_Add(%TArrayList* %t.5, i32 15)

	%t.6 = load %TArrayList*, %TArrayList** %t.0
	call void @Al_Add(%TArrayList* %t.6, i32 16)

	%t.7 = load %TArrayList*, %TArrayList** %t.0
	call void @Al_Add(%TArrayList* %t.7, i32 5)

	%t.8 = load %TArrayList*, %TArrayList** %t.0
	call void @Al_Add(%TArrayList* %t.8, i32 6)

	%t.9 = load %TArrayList*, %TArrayList** %t.0
	call void @Al_Add(%TArrayList* %t.9, i32 5)

	%t.10 = load %TArrayList*, %TArrayList** %t.0
	%t.12 = sub i32 0, 8
	call void @Al_Add(%TArrayList* %t.10, i32 %t.12)

	%t.13 = load %TArrayList*, %TArrayList** %t.0
	call void @Al_Print(%TArrayList* %t.13)

	ret void
}




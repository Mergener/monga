source_filename = "tests/codegen_cases/arraylist.mon"

%string = type { i32, i32 }

declare %string* @RtInternal_StrFromSZ(i8*)
declare i8* @RtInternal_GcAlloc(i32)
declare void @RtInternal_Init()

%TArrayList = type { i32*, i32, i32 }

define %TArrayList* @Al_Create() {
	%t.0 = alloca %TArrayList*

	store %TArrayList* null, %TArrayList** %t.0
	%t.1 = call i8* @RtInternal_GcAlloc(i32 16)
	%t.2 = bitcast i8* %t.1 to %TArrayList*
	store %TArrayList* %t.2, %TArrayList** %t.0

	%t.3 = load %TArrayList*, %TArrayList** %t.0
	%t.4 = getelementptr inbounds %TArrayList, %TArrayList* %t.3, i32 0, i32 1
	store i32 0, i32* %t.4

	%t.5 = load %TArrayList*, %TArrayList** %t.0
	%t.6 = getelementptr inbounds %TArrayList, %TArrayList* %t.5, i32 0, i32 0
	%t.7 = mul i32 8, 4
	%t.8 = call i8* @RtInternal_GcAlloc(i32 %t.7)
	%t.9 = bitcast i8* %t.8 to i32*
	store i32* %t.9, i32** %t.6

	%t.10 = load %TArrayList*, %TArrayList** %t.0
	%t.11 = getelementptr inbounds %TArrayList, %TArrayList* %t.10, i32 0, i32 2
	store i32 8, i32* %t.11

	%t.12 = load %TArrayList*, %TArrayList** %t.0
	ret %TArrayList* %t.12
}

define void @Al_Resize(%TArrayList*, i32) {
	%t.0 = alloca %TArrayList*
	store %TArrayList* %0, %TArrayList** %t.0
	%t.1 = alloca i32
	store i32 %1, i32* %t.1

	%t.2 = alloca i32*

	store i32* null, i32** %t.2
	%t.3 = alloca i32

	store i32 0, i32* %t.3
	%t.4 = alloca i32

	store i32 0, i32* %t.4
	%t.5 = load i32, i32* %t.1
	%t.6 = mul i32 %t.5, 4
	%t.7 = call i8* @RtInternal_GcAlloc(i32 %t.6)
	%t.8 = bitcast i8* %t.7 to i32*
	store i32* %t.8, i32** %t.2

	%t.9 = load i32, i32* %t.1
	%t.10 = load %TArrayList*, %TArrayList** %t.0
	%t.11 = getelementptr inbounds %TArrayList, %TArrayList* %t.10, i32 0, i32 1
	%t.12 = load i32, i32* %t.11
	%t.13 = icmp slt i32 %t.9, %t.12
	br i1 %t.13, label %l.0, label %l.1
l.0:
	%t.14 = load i32, i32* %t.1
	br label %l.3
l.3:
	br label %l.2
l.1:
	%t.15 = load %TArrayList*, %TArrayList** %t.0
	%t.16 = getelementptr inbounds %TArrayList, %TArrayList* %t.15, i32 0, i32 1
	%t.17 = load i32, i32* %t.16
	br label %l.4
l.4:
	br label %l.2
l.2:
	%t.18 = phi i32 [%t.14, %l.3], [%t.17, %l.4]
	store i32 %t.18, i32* %t.4

	store i32 0, i32* %t.3

	%t.19 = load i32, i32* %t.3
	%t.20 = load i32, i32* %t.4
	%t.21 = icmp slt i32 %t.19, %t.20
	br i1 %t.21, label %l.6, label %l.5
l.6:
	%t.22 = load i32, i32* %t.3
	%t.23 = sext i32 %t.22 to i64
	%t.24 = load i32*, i32** %t.2
	%t.25 = getelementptr i32, i32* %t.24, i64 %t.23
	%t.26 = load i32, i32* %t.3
	%t.27 = sext i32 %t.26 to i64
	%t.28 = load %TArrayList*, %TArrayList** %t.0
	%t.29 = getelementptr inbounds %TArrayList, %TArrayList* %t.28, i32 0, i32 0
	%t.30 = load i32*, i32** %t.29
	%t.31 = getelementptr i32, i32* %t.30, i64 %t.27
	%t.32 = load i32, i32* %t.31
	store i32 %t.32, i32* %t.25

	%t.33 = load i32, i32* %t.3
	%t.34 = add i32 %t.33, 1
	store i32 %t.34, i32* %t.3

	br label %l.7
l.7:
	%t.35 = load i32, i32* %t.3
	%t.36 = load i32, i32* %t.4
	%t.37 = icmp slt i32 %t.35, %t.36
	br i1 %t.37, label %l.6, label %l.5
l.5:
	%t.38 = load %TArrayList*, %TArrayList** %t.0
	%t.39 = getelementptr inbounds %TArrayList, %TArrayList* %t.38, i32 0, i32 2
	%t.40 = load i32, i32* %t.1
	store i32 %t.40, i32* %t.39

	%t.41 = load %TArrayList*, %TArrayList** %t.0
	%t.42 = getelementptr inbounds %TArrayList, %TArrayList* %t.41, i32 0, i32 0
	%t.43 = load i32*, i32** %t.2
	store i32* %t.43, i32** %t.42

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
	%t.20 = load i32*, i32** %t.19
	%t.21 = getelementptr i32, i32* %t.20, i64 %t.17
	%t.22 = load i32, i32* %t.1
	store i32 %t.22, i32* %t.21

	%t.23 = load %TArrayList*, %TArrayList** %t.0
	%t.24 = getelementptr inbounds %TArrayList, %TArrayList* %t.23, i32 0, i32 1
	%t.25 = load %TArrayList*, %TArrayList** %t.0
	%t.26 = getelementptr inbounds %TArrayList, %TArrayList* %t.25, i32 0, i32 1
	%t.27 = load i32, i32* %t.26
	%t.28 = add i32 %t.27, 1
	store i32 %t.28, i32* %t.24

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
	%t.39 = load i32*, i32** %t.38
	%t.40 = getelementptr i32, i32* %t.39, i64 %t.36
	%t.41 = load i32, i32* %t.2
	%t.42 = add i32 %t.41, 1
	%t.43 = sext i32 %t.42 to i64
	%t.44 = load %TArrayList*, %TArrayList** %t.0
	%t.45 = getelementptr inbounds %TArrayList, %TArrayList* %t.44, i32 0, i32 0
	%t.46 = load i32*, i32** %t.45
	%t.47 = getelementptr i32, i32* %t.46, i64 %t.43
	%t.48 = load i32, i32* %t.47
	store i32 %t.48, i32* %t.40

	%t.49 = load i32, i32* %t.2
	%t.50 = add i32 %t.49, 1
	store i32 %t.50, i32* %t.2

	br label %l.7
l.7:
	%t.51 = load i32, i32* %t.2
	%t.52 = load %TArrayList*, %TArrayList** %t.0
	%t.53 = getelementptr inbounds %TArrayList, %TArrayList* %t.52, i32 0, i32 1
	%t.54 = load i32, i32* %t.53
	%t.55 = icmp slt i32 %t.51, %t.54
	br i1 %t.55, label %l.6, label %l.5
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
	%t.15 = load i32*, i32** %t.14
	%t.16 = getelementptr i32, i32* %t.15, i64 %t.12
	%t.17 = load i32, i32* %t.16
	ret i32 %t.17
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
	call void @printInteger(i32 %t.11)

	%t.12 = load i32, i32* %t.5
	%t.13 = add i32 %t.12, 1
	store i32 %t.13, i32* %t.5

	br label %l.2
l.2:
	%t.14 = load i32, i32* %t.5
	%t.15 = load i32, i32* %t.1
	%t.16 = icmp slt i32 %t.14, %t.15
	br i1 %t.16, label %l.1, label %l.0
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

declare void @printInteger(i32)



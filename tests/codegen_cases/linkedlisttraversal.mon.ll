source_filename = "tests/codegen_cases/linkedlisttraversal.mon"

%string = type { i32, i32 }

declare %string* @RtInternal_StrFromSZ(i8*)
declare i8* @RtInternal_GcAlloc(i32)
declare void @RtInternal_Init()

%Node = type { %Node*, i8 }

define %Node* @makeNode(%Node*, i8) {
	%t.0 = alloca %Node*
	store %Node* %0, %Node** %t.0
	%t.1 = alloca i8
	store i8 %1, i8* %t.1

	%t.2 = alloca %Node*

	store %Node* null, %Node** %t.2
	%t.3 = call i8* @RtInternal_GcAlloc(i32 16)
	%t.4 = bitcast i8* %t.3 to %Node*
	store %Node* %t.4, %Node** %t.2

	%t.5 = load %Node*, %Node** %t.2
	%t.6 = getelementptr inbounds %Node, %Node* %t.5, i32 0, i32 0
	%t.7 = load %Node*, %Node** %t.0
	store %Node* %t.7, %Node** %t.6

	%t.8 = load %Node*, %Node** %t.2
	%t.9 = getelementptr inbounds %Node, %Node* %t.8, i32 0, i32 1
	%t.10 = load i8, i8* %t.1
	store i8 %t.10, i8* %t.9

	%t.11 = load %Node*, %Node** %t.2
	ret %Node* %t.11
}

define void @printList(%Node*) {
	%t.0 = alloca %Node*
	store %Node* %0, %Node** %t.0

	%t.1 = load %Node*, %Node** %t.0
	%t.2 = icmp ne %Node* %t.1, null
	br i1 %t.2, label %l.1, label %l.0
l.1:
	%t.3 = load %Node*, %Node** %t.0
	%t.4 = getelementptr inbounds %Node, %Node* %t.3, i32 0, i32 1
	%t.5 = load i8, i8* %t.4
	%t.6 = sext i8 %t.5 to i32
	call void @printInteger(i32 %t.6)

	%t.7 = load %Node*, %Node** %t.0
	%t.8 = getelementptr inbounds %Node, %Node* %t.7, i32 0, i32 0
	%t.9 = load %Node*, %Node** %t.8
	store %Node* %t.9, %Node** %t.0

	br label %l.2
l.2:
	%t.10 = load %Node*, %Node** %t.0
	%t.11 = icmp ne %Node* %t.10, null
	br i1 %t.11, label %l.1, label %l.0
l.0:
	ret void
}

define void @main() {
	call void @RtInternal_Init()
	%t.0 = alloca %Node*

	store %Node* null, %Node** %t.0
	store %Node* null, %Node** %t.0

	%t.1 = alloca i64

	store i64 0, i64* %t.1
	%t.2 = sext i32 20 to i64
	store i64 %t.2, i64* %t.1

	%t.3 = load i64, i64* %t.1
	%t.4 = sext i32 0 to i64
	%t.5 = icmp sge i64 %t.3, %t.4
	br i1 %t.5, label %l.1, label %l.0
l.1:
	%t.6 = load %Node*, %Node** %t.0
	%t.7 = load i64, i64* %t.1
	%t.8 = trunc i64 %t.7 to i8
	%t.9 = call %Node* @makeNode(%Node* %t.6, i8 %t.8)
	store %Node* %t.9, %Node** %t.0

	%t.10 = load i64, i64* %t.1
	%t.11 = sext i32 1 to i64
	%t.12 = sub i64 %t.10, %t.11
	store i64 %t.12, i64* %t.1

	br label %l.2
l.2:
	%t.13 = load i64, i64* %t.1
	%t.14 = sext i32 0 to i64
	%t.15 = icmp sge i64 %t.13, %t.14
	br i1 %t.15, label %l.1, label %l.0
l.0:
	%t.16 = load %Node*, %Node** %t.0
	call void @printList(%Node* %t.16)

	ret void
}

declare void @printInteger(i32)



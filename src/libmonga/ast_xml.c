#include "ast_dump.h"

#include <assert.h>

#include "ast/definitions/mon_type_def.h"

/** Dumps a single parameter node. */
static void XmlDumpParam(AstDumpContext* ctx, const Mon_AstParam* param);

/** Dumps all chained parameter nodes. */
static void XmlDumpParams(AstDumpContext* ctx, const Mon_Vector* params);

/** Dumps a function node. */
static void XmlDumpFuncNode(AstDumpContext* ctx, const Mon_AstFuncDef* funcDef);

/** Dumps a variable declaration node. */
static void XmlDumpVarDefNode(AstDumpContext* ctx, const Mon_AstVarDef* varDef);

/** Dumps a type declaration node. */
static void XmlDumpTypeNode(AstDumpContext* ctx, const Mon_AstTypeDef* typeDef);

/** Dumps a definition node - will either be a vardef, typedef or funcdef. */
static void XmlDumpDefNode(AstDumpContext* ctx, const Mon_AstDef* defNode);

/** Dumps a block without enclosing tags. */
static void XmlDumpBlockInline(AstDumpContext* ctx, const Mon_AstBlock* block);

/** Dumps a condition node. */
static void XmlDumpCondNode(AstDumpContext* ctx, const Mon_AstCond* cond);

/** Dumps an expression node. */
static void XmlDumpExpNode(AstDumpContext* ctx, const Mon_AstExp* exp);

/** Dumps a variable reference node. */
static void XmlDumpVarNode(AstDumpContext* ctx, const Mon_AstVar* var);

/** Dumps a function call node. */
static void XmlDumpCallNode(AstDumpContext* ctx, const Mon_AstCall* call);

void AstDumpXml(AstDumpContext* ctx, const Mon_Ast* ast) {
    assert(ctx != NULL);
    assert(ast != NULL);

    DUMPF_OR_STOP(ctx, "<Module>");
    ctx->indentLevel++;

    MON_VECTOR_FOREACH(&ast->defsVector, Mon_AstDef*, def,
        XmlDumpDefNode(ctx, def);
        if (ctx->ioErr) {
            return;
        }
    );

    ctx->indentLevel--;
    DUMPF_OR_STOP(ctx, "</Module>");
}

static void XmlDumpVarNode(AstDumpContext* ctx, const Mon_AstVar* var) {
    assert(ctx != NULL);
    assert(var != NULL);

    switch (var->varKind) {
        case MON_VAR_DIRECT:
            DUMPF_OR_STOP(ctx, "<Var>%s</Var>", var->var.direct);
            break;

        case MON_VAR_FIELD:
            DUMPF_OR_STOP(ctx, "<Field>");
            ctx->indentLevel++;
            
            DUMPF_OR_STOP(ctx, "<AccessedExp>");
            ctx->indentLevel++;
            XmlDumpExpNode(ctx, var->var.field.expr);
            ctx->indentLevel--;
            DUMPF_OR_STOP(ctx, "</AccessedExp>");

            DUMPF_OR_STOP(ctx, "<FieldName>%s</FieldName>", var->var.field.fieldName);

            ctx->indentLevel--;
            DUMPF_OR_STOP(ctx, "</Field>");
            break;

        case MON_VAR_INDEXED:
            DUMPF_OR_STOP(ctx, "<Indexed>");
            ctx->indentLevel++;
            
            DUMPF_OR_STOP(ctx, "<IndexedExp>");
            ctx->indentLevel++;
            XmlDumpExpNode(ctx, var->var.indexed.indexedExpr);
            ctx->indentLevel--;
            DUMPF_OR_STOP(ctx, "</IndexedExp>");
            
            DUMPF_OR_STOP(ctx, "<IndexExp>");
            ctx->indentLevel++;
            XmlDumpExpNode(ctx, var->var.indexed.indexExpr);
            ctx->indentLevel--;
            DUMPF_OR_STOP(ctx, "</IndexExp>");

            ctx->indentLevel--;
            DUMPF_OR_STOP(ctx, "</Indexed>");
            break;
    }
}

static void XmlDumpExpUnopNode(AstDumpContext* ctx, const Mon_AstExp* exp) {
    assert(ctx != NULL);
    assert(exp != NULL);
    assert(exp->expKind == MON_EXP_UNOP);

    switch (exp->exp.unaryOperation.unOpKind) {
        case MON_UNOP_NEGATIVE:
            DUMPF_OR_STOP(ctx, "<OppositeSign>");
            ctx->indentLevel++;

            XmlDumpExpNode(ctx, exp->exp.unaryOperation.operand);

            ctx->indentLevel--;
            DUMPF_OR_STOP(ctx, "</OppositeSign>");
            break;

        case MON_UNOP_BITNOT:
            DUMPF_OR_STOP(ctx, "<BitwiseNot>");
            ctx->indentLevel++;

            XmlDumpExpNode(ctx, exp->exp.unaryOperation.operand);

            ctx->indentLevel--;
            DUMPF_OR_STOP(ctx, "</BitwiseNot>");
            break;
    }
}

static void XmlDumpBinop(AstDumpContext* ctx, const Mon_AstExp* exp, const char* binopName) {
    assert(ctx != NULL);
    assert(exp != NULL);
    assert(binopName != NULL);

    DUMPF_OR_STOP(ctx, "<%s>", binopName);
    ctx->indentLevel++;
    
    DUMPF_OR_STOP(ctx, "<OperandA>");
    ctx->indentLevel++;
    XmlDumpExpNode(ctx, exp->exp.binaryOperation.left);
    ctx->indentLevel--;
    DUMPF_OR_STOP(ctx, "</OperandA>");

    DUMPF_OR_STOP(ctx, "<OperandB>");
    ctx->indentLevel++;
    XmlDumpExpNode(ctx, exp->exp.binaryOperation.right);
    ctx->indentLevel--;
    DUMPF_OR_STOP(ctx, "</OperandB>");

    ctx->indentLevel--;
    DUMPF_OR_STOP(ctx, "</%s>", binopName);
}

static void XmlDumpExpBinopNode(AstDumpContext* ctx, const Mon_AstExp* exp) {
    assert(ctx != NULL);
    assert(exp != NULL);
    assert(exp->expKind == MON_EXP_BINOP);

    switch (exp->exp.binaryOperation.binOpKind) {
        case MON_BINOP_ADD:
            XmlDumpBinop(ctx, exp, "Addition");
            break;
            
        case MON_BINOP_SUB:
            XmlDumpBinop(ctx, exp, "Subtraction");
            break;

        case MON_BINOP_MUL:
            XmlDumpBinop(ctx, exp, "Multiplication");
            break;

        case MON_BINOP_DIV:
            XmlDumpBinop(ctx, exp, "Division");
            break;

        case MON_BINOP_MODULO:
            XmlDumpBinop(ctx, exp, "Modulo");
            break;

        case MON_BINOP_SHR:
            XmlDumpBinop(ctx, exp, "RightShift");
            break;

        case MON_BINOP_SHL:
            XmlDumpBinop(ctx, exp, "LeftShift");
            break;

        case MON_BINOP_BITAND:
            XmlDumpBinop(ctx, exp, "BitwiseAnd");
            break;

        case MON_BINOP_BITOR:
            XmlDumpBinop(ctx, exp, "BitwiseOr");
            break;

        case MON_BINOP_XOR:
            XmlDumpBinop(ctx, exp, "BitwiseXor");
            break;
    }
}

static void XmlDumpLiteral(AstDumpContext* ctx, Mon_Literal literal) {
    assert(ctx != NULL);

    switch (literal.literalKind) {
        case MON_LIT_STR:
            assert(literal.string.arr != NULL);
            DUMPF_OR_STOP(ctx, "<StringConstant>%s</StringConstant>", literal.string.arr);
            break;

        case MON_LIT_FLOAT:
            DUMPF_OR_STOP(ctx, "<FloatConstant>%f</FloatConstant>", literal.real);
            break;

        case MON_LIT_INT:
            DUMPF_OR_STOP(ctx, "<IntConstant>%ld</IntConstant>", literal.integer);
            break;
    }
}

static void XmlDumpExpNode(AstDumpContext* ctx, const Mon_AstExp* exp) {
    assert(ctx != NULL);
    assert(exp != NULL);

    switch (exp->expKind) {
        case MON_EXP_CONDITIONAL:
            DUMPF_OR_STOP(ctx, "<ConditionalExp>");
            ctx->indentLevel++;

            XmlDumpCondNode(ctx, exp->exp.conditionalExpr.condition);

            DUMPF_OR_STOP(ctx, "<ThenExp>");
            ctx->indentLevel++;
            XmlDumpExpNode(ctx, exp->exp.conditionalExpr.thenExpr);
            ctx->indentLevel--;
            DUMPF_OR_STOP(ctx, "</ThenExp>");

            DUMPF_OR_STOP(ctx, "<ElseExp>");
            ctx->indentLevel++;
            XmlDumpExpNode(ctx, exp->exp.conditionalExpr.elseExpr);
            ctx->indentLevel--;
            DUMPF_OR_STOP(ctx, "</ElseExp>");

            ctx->indentLevel--;
            DUMPF_OR_STOP(ctx, "</ConditionalExp>");
            break;

        case MON_EXP_VAR:
            XmlDumpVarNode(ctx, exp->exp.varExpr);
            break;

        case MON_EXP_UNOP:
            XmlDumpExpUnopNode(ctx, exp);
            break;

        case MON_EXP_BINOP:
            XmlDumpExpBinopNode(ctx, exp);
            break;

        case MON_EXP_CAST:
            DUMPF_OR_STOP(ctx, "<TypeCast>");
            ctx->indentLevel++;

            DUMPF_OR_STOP(ctx, "<Type>%s</Type>", exp->exp.castExpr.typeName);

            DUMPF_OR_STOP(ctx, "<CastedExp>");
            ctx->indentLevel++;
            XmlDumpExpNode(ctx, exp->exp.castExpr.castee);
            ctx->indentLevel--;
            DUMPF_OR_STOP(ctx, "</CastedExp>");

            ctx->indentLevel--;
            DUMPF_OR_STOP(ctx, "</TypeCast>");
            break;

        case MON_EXP_NEW:
            DUMPF_OR_STOP(ctx, "<New>");
            ctx->indentLevel++;

            DUMPF_OR_STOP(ctx, "<Type>%s</Type>", exp->exp.newExpr.typeName);
            if (exp->exp.newExpr.arraySizeExp != NULL) {
                DUMPF_OR_STOP(ctx, "<ArraySize>");
                ctx->indentLevel++;

                XmlDumpExpNode(ctx, exp->exp.newExpr.arraySizeExp);

                ctx->indentLevel--;
                DUMPF_OR_STOP(ctx, "</ArraySize>");
            }

            ctx->indentLevel--;
            DUMPF_OR_STOP(ctx, "</New>");
            break;

        case MON_EXP_LITERAL:
            XmlDumpLiteral(ctx, exp->exp.literalExpr);
            break;

        case MON_EXP_CALL:
            XmlDumpCallNode(ctx, exp->exp.callExpr);
            break;
    }
}

static void XmlDumpCompar(AstDumpContext* ctx, const Mon_AstCond* cond, const char* comparName) {
    assert(cond->condKind == MON_COND_COMPARISON);

    DUMPF_OR_STOP(ctx, "<%s>", comparName);
    ctx->indentLevel++;

    XmlDumpExpNode(ctx, cond->condition.compar.left);
    XmlDumpExpNode(ctx, cond->condition.compar.right);

    ctx->indentLevel--;
    DUMPF_OR_STOP(ctx, "</%s>", comparName);
}

static void XmlDumpBinCond(AstDumpContext* ctx, const Mon_AstCond* cond, const char* binCondName) {
    assert(cond->condKind == MON_COND_BIN);

    DUMPF_OR_STOP(ctx, "<%s>", binCondName);
    ctx->indentLevel++;

    XmlDumpCondNode(ctx, cond->condition.binCond.left);
    XmlDumpCondNode(ctx, cond->condition.binCond.right);

    ctx->indentLevel--;
    DUMPF_OR_STOP(ctx, "</%s>", binCondName);
}

static void XmlDumpCondNode(AstDumpContext* ctx, const Mon_AstCond* cond) {
    assert(ctx != NULL);
    assert(cond != NULL);

    DUMPF_OR_STOP(ctx, "<Condition>");
    ctx->indentLevel++;

    if (cond->negate) {
        DUMPF_OR_STOP(ctx, "<Not>");
        ctx->indentLevel++;
    }

    switch (cond->condKind) {
        case MON_COND_BIN:
            switch (cond->condition.binCond.binCondKind) {
                case MON_BINCOND_AND:
                    XmlDumpBinCond(ctx, cond, "And");
                    break;

                case MON_BINCOND_OR:
                    XmlDumpBinCond(ctx, cond, "Or");
                    break;
            }
            break;

        case MON_COND_COMPARISON:
            switch (cond->condition.compar.comparKind) {
                case MON_COMPAR_GT:
                    XmlDumpCompar(ctx, cond, "GreaterThan");
                    break;

                case MON_COMPAR_GE:
                    XmlDumpCompar(ctx, cond, "GreaterThanOrEqualTo");
                    break;

                case MON_COMPAR_EQ:
                    XmlDumpCompar(ctx, cond, "EqualTo");
                    break;

                case MON_COMPAR_NE:
                    XmlDumpCompar(ctx, cond, "NotEqualTo");
                    break;

                case MON_COMPAR_LE:
                    XmlDumpCompar(ctx, cond, "LessThanOrEqualTo");
                    break;

                case MON_COMPAR_LT:
                    XmlDumpCompar(ctx, cond, "LessThan");
                    break;
            }
            break;
    }

    if (cond->negate) {
        DUMPF_OR_STOP(ctx, "</Not>");
        ctx->indentLevel--;
    }

    ctx->indentLevel--;
    DUMPF_OR_STOP(ctx, "</Condition>");
}


static void XmlDumpCallNode(AstDumpContext* ctx, const Mon_AstCall* call) {
    DUMPF_OR_STOP(ctx, "<Call>");
    ctx->indentLevel++;

    DUMPF_OR_STOP(ctx, "<Callee>%s</Callee>", call->funcName);
    
    if (!Mon_VectorEmpty(&call->parameterList)) {
        DUMPF_OR_STOP(ctx, "<Parameters>");
        ctx->indentLevel++;

        MON_VECTOR_FOREACH(&call->parameterList, Mon_AstExp*, exp, 
            DUMPF_OR_STOP(ctx, "<Parameter>");
            ctx->indentLevel++;

            XmlDumpExpNode(ctx, exp);

            ctx->indentLevel--;
            DUMPF_OR_STOP(ctx, "</Parameter>");
        );

        ctx->indentLevel--;
        DUMPF_OR_STOP(ctx, "</Parameters>");
    }

    ctx->indentLevel--;
    DUMPF_OR_STOP(ctx, "</Call>");
}

static void XmlDumpStatementNode(AstDumpContext* ctx, const Mon_AstStatement* stmt) {
    switch (stmt->statementKind) {
        case MON_STMT_IF:
            DUMPF_OR_STOP(ctx, "<If>");
            ctx->indentLevel++;

            XmlDumpCondNode(ctx, stmt->statement.ifStmt.condition);

            if (stmt->statement.ifStmt.thenBlock != NULL) {
                DUMPF_OR_STOP(ctx, "<Then>")
                ctx->indentLevel++;

                XmlDumpBlockInline(ctx, stmt->statement.ifStmt.thenBlock);

                ctx->indentLevel--;
                DUMPF_OR_STOP(ctx, "</Then>")
            }

            if (stmt->statement.ifStmt.elseBlock != NULL) {
                DUMPF_OR_STOP(ctx, "<Else>")
                ctx->indentLevel++;

                XmlDumpBlockInline(ctx, stmt->statement.ifStmt.elseBlock);
                
                ctx->indentLevel--;
                DUMPF_OR_STOP(ctx, "</Else>")
            }

            ctx->indentLevel--;
            DUMPF_OR_STOP(ctx, "</If>");
            break;

        case MON_STMT_WHILE:
            DUMPF_OR_STOP(ctx, "<While>");
            ctx->indentLevel++;

            XmlDumpCondNode(ctx, stmt->statement.whileStmt.condition);

            if (stmt->statement.whileStmt.block != NULL) {
                DUMPF_OR_STOP(ctx, "<Do>")
                ctx->indentLevel++;

                XmlDumpBlockInline(ctx, stmt->statement.whileStmt.block);
                
                ctx->indentLevel--;
                DUMPF_OR_STOP(ctx, "</Do>")
            }

            ctx->indentLevel--;
            DUMPF_OR_STOP(ctx, "</While>");
            break;

        case MON_STMT_ASSIGNMENT:
            DUMPF_OR_STOP(ctx, "<Assignment>");
            ctx->indentLevel++;

            XmlDumpVarNode(ctx, stmt->statement.assignment.lvalue);
            XmlDumpExpNode(ctx, stmt->statement.assignment.rvalue);

            ctx->indentLevel--;
            DUMPF_OR_STOP(ctx, "</Assignment>");
            break;

        case MON_STMT_RETURN:
            DUMPF_OR_STOP(ctx, "<Return>");
            ctx->indentLevel++;

            if (stmt->statement.returnStmt.returnedExpression != NULL) {
                XmlDumpExpNode(ctx, stmt->statement.returnStmt.returnedExpression);
            }

            ctx->indentLevel--;
            DUMPF_OR_STOP(ctx, "</Return>");
            break;

        case MON_STMT_CALL:
            XmlDumpCallNode(ctx, stmt->statement.call);
            break;

        case MON_STMT_BLOCK:
            DUMPF_OR_STOP(ctx, "<Block>");
            ctx->indentLevel++;

            XmlDumpBlockInline(ctx, stmt->statement.block);

            ctx->indentLevel--;
            DUMPF_OR_STOP(ctx, "</Block>");
            break;
    }
}

static void XmlDumpBlockInline(AstDumpContext* ctx, const Mon_AstBlock* block) {
    if (block == NULL) {
        return;
    }

    MON_VECTOR_FOREACH(&block->varDefs, Mon_AstVarDef*, varDef,
        XmlDumpVarDefNode(ctx, varDef);
    );

    MON_VECTOR_FOREACH(&block->statements, Mon_AstStatement*, stmt,
        XmlDumpStatementNode(ctx, stmt);
    );
}

static void XmlDumpParam(AstDumpContext* ctx, const Mon_AstParam* param) {
    DUMPF_OR_STOP(ctx, "<Parameter>");
    ctx->indentLevel++;

    DUMPF_OR_STOP(ctx, "<Name>%s</Name>", param->name);
    DUMPF_OR_STOP(ctx, "<Type>%s</Type>", param->typeName);

    ctx->indentLevel--;
    DUMPF_OR_STOP(ctx, "</Parameter>");
}

static void XmlDumpParams(AstDumpContext* ctx, const Mon_Vector* params) {
    DUMPF_OR_STOP(ctx, "<Parameters>");
    ctx->indentLevel++;

    MON_VECTOR_FOREACH(params, Mon_AstParam*, param, 
        XmlDumpParam(ctx, param);
    );

    ctx->indentLevel--;
    DUMPF_OR_STOP(ctx, "</Parameters>");
}

static void XmlDumpFuncNode(AstDumpContext* ctx, const Mon_AstFuncDef* funcDef) {
    DUMPF_OR_STOP(ctx, "<FunctionDefinition>");
    ctx->indentLevel++;

    DUMPF_OR_STOP(ctx, "<Name>%s</Name>", funcDef->funcName);
    DUMPF_OR_STOP(ctx, "<ReturnType>%s</ReturnType>", funcDef->funcRetTypeName);

    if (!Mon_VectorEmpty(&funcDef->parameters)) {
        XmlDumpParams(ctx, &funcDef->parameters);
    }

    DUMPF_OR_STOP(ctx, "<Block>");
    ctx->indentLevel++;

    XmlDumpBlockInline(ctx, funcDef->body);

    ctx->indentLevel--;
    DUMPF_OR_STOP(ctx, "</Block>");

    ctx->indentLevel--;
    DUMPF_OR_STOP(ctx, "</FunctionDefinition>");
}

static void XmlDumpVarDefNode(AstDumpContext* ctx, const Mon_AstVarDef* varDef) {
    DUMPF_OR_STOP(ctx, "<VariableDeclaration>");
    ctx->indentLevel++;

    DUMPF_OR_STOP(ctx, "<Name>%s</Name>", varDef->varName);
    DUMPF_OR_STOP(ctx, "<Type>%s</Type>", varDef->typeName);

    ctx->indentLevel--;
    DUMPF_OR_STOP(ctx, "</VariableDeclaration>");
}

static void XmlDumpTypeNode(AstDumpContext* ctx, const Mon_AstTypeDef* typeDef) {
    DUMPF_OR_STOP(ctx, "<TypeDefinition>");
    ctx->indentLevel++;

    DUMPF_OR_STOP(ctx, "<Name>%s</Name>", typeDef->typeName);

    ctx->indentLevel--;
    DUMPF_OR_STOP(ctx, "</TypeDefinition>");
}

static void XmlDumpDefNode(AstDumpContext* ctx, const Mon_AstDef* defNode) {
    switch (defNode->defKind) {
        case MON_AST_DEF_FUNC:
            XmlDumpFuncNode(ctx, defNode->definition.function);
            break;

        case MON_AST_DEF_VAR:
            XmlDumpVarDefNode(ctx, defNode->definition.variable);			
            break;

        case MON_AST_DEF_TYPE:
            XmlDumpTypeNode(ctx, defNode->definition.type);
            break;

        default:
            break;
    }
}
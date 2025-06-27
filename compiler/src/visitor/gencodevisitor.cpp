#include <iostream>
#include <unordered_map>
#include "exp/exp.h"
#include "visitor.h"
using namespace std;

void GenCodeVisitor::generar(Program* program) {
    program->accept(this);
}

int GenCodeVisitor::visit(Program* program) {
    out << ".data\n"
        << "print_fmt:     .string \"%ld \"\n"
        << "print_fmt_ln:  .string \"%ld \\n\"\n";
    program->vl->accept(this);

    for (auto& [var, _] : memoriaGlobal) {
        out << var << ": .quad 0" << endl;
    }

    out << ".text\n";
    if (program->cl)
        program->cl->accept(this);
    if (program->fl)
        program->fl->accept(this);
    out << ".section .note.GNU-stack,\"\",@progbits" << endl;
    return 0;
}

int GenCodeVisitor::visit(VarDec* s) {
    if (entornoClase) {
        if (!s->stm) {
            out << " movq $0, " << clases[nombreClase].off[s->var] << "(%rdi)" << endl;
        } else {
            s->stm->accept(this);
            // out << " movq %rax, " << clases[nombreClase].off[s->var] << "(%rdi)" << endl;
            // memoria[s->var] = offset;
            // offset -= 8;
        }
    } else {
        if (!entornoFuncion) {
            memoriaGlobal[s->var] = true;

            if (s->stm) {
                s->stm->accept(this);
                out << " movq %rax, " << s->var << "(%rip)\n";
            }
            return 0;
        }
        out << " subq $8, %rsp\n";

        memoria[s->var] = offset;
        offset -= 8;

        if (s->stm) {
            s->stm->accept(this);
            // out << " movq %rax, " << memoria[s->var] << "(%rbp)\n";
            if (auto fcall = dynamic_cast<FCallExp*>(s->stm->rhs)) {
                if (fcall->is_class) {
                    tipoClase[s->var] = fcall->nombre;
                }
            }
        } else {
            out << " movq $0, " << memoria[s->var] << "(%rbp)\n";
        }
    }

    return 0;
}

int GenCodeVisitor::visit(VarDecList* stm) {
    for (auto dec : stm->vardecs)
        dec->accept(this);
    return 0;
}

int GenCodeVisitor::visit(ClassDec* cd) {
    entornoClase = true;
    nombreClase = cd->id;

    int local_offset = 0;
    ClassInfo info;
    vector<std::string> argRegs = {"%rsi", "%rdx", "%rcx", "%r8", "%r9"};

    out << ".globl " << cd->id << "$ctor\n";
    out << cd->id << "$ctor:\n";

    int i = 0;
    for (auto p : cd->pl->param_list) {
        info.off[p->id] = local_offset;
        out << " movq " << argRegs[i] << ", " << local_offset << "(%rdi)" << endl;
        local_offset += 8;
        i++;
    }

    for (auto v : cd->vdl->vardecs) {
        info.off[v->var] = local_offset;
        local_offset += 8;
    }

    info.size = local_offset;
    clases[cd->id] = info;

    cd->vdl->accept(this);
    out << "ret" << endl;
    entornoClase = false;
    return 0;
}

int GenCodeVisitor::visit(ClassDecList* cdl) {
    for (auto cd : cdl->classdecs) {
        cd->accept(this);
    }
    return 0;
}

int GenCodeVisitor::visit(FunDec* f) {
    entornoFuncion = true;
    memoria.clear();
    offset = -8;
    nombreFuncion = f->id;
    vector<std::string> argRegs = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    out << ".globl " << f->id << endl;
    out << f->id << ":" << endl;
    out << " pushq %rbp" << endl;
    out << " movq %rsp, %rbp" << endl;
    int size = f->paramList->param_list.size();
    for (int i = 0; i < size; i++) {
        memoria[f->paramList->param_list[i]->id] = offset;
        out << " movq " << argRegs[i] << "," << offset << "(%rbp)" << endl;
        offset -= 8;
    }

    f->block->vardecl->accept(this);
    int reserva = (-offset);
    reserva = (reserva + 15) & ~15;
    if (reserva) {
        out << " subq $" << reserva << ", %rsp" << endl;
    }
    f->block->stmdecl->accept(this);

    out << ".end_" << f->id << ":" << endl;
    out << "leave" << endl;
    out << "ret" << endl;
    entornoFuncion = false;
    return 0;
}

int GenCodeVisitor::visit(IdentifierExp* exp) {
    if (entornoClase) {
        cout << clases[nombreClase].off[exp->name] << endl;
        out << " movq " << clases[nombreClase].off[exp->name] << "(%rdi), %rax" << endl;
    } else {
        if (memoriaGlobal.count(exp->name))
            out << " movq " << exp->name << "(%rip), %rax" << endl;
        else
            out << " movq " << memoria[exp->name] << "(%rbp), %rax" << endl;
    }
    return 0;
}

int GenCodeVisitor::visit(BinaryExp* exp) {
    exp->left->accept(this);
    out << " pushq %rax\n";
    exp->right->accept(this);
    out << " movq %rax, %rcx\n popq %rax\n";
    switch (exp->op) {
        case PLUS_OP:
            out << " addq %rcx, %rax\n";
            break;
        case MINUS_OP:
            out << " subq %rcx, %rax\n";
            break;
        case MUL_OP:
            out << " imulq %rcx, %rax\n";
            break;
        case DIV_OP:
            out << " cqto\n"
                   " idivq %rcx\n";
            break;
        case LT_OP:
            out << " cmpq %rcx, %rax\n"
                   " movl $0, %eax\n"
                   " setl %al\n"
                   " movzbq %al, %rax\n";
            break;
        case LE_OP:
            out << " cmpq %rcx, %rax\n"
                   " movl $0, %eax\n"
                   " setle %al\n"
                   " movzbq %al, %rax\n";
            break;
        case GT_OP:
            out << " cmpq %rcx, %rax\n"
                   " movl $0, %eax\n"
                   " setg %al\n"
                   " movzbq %al, %rax\n";
            break;
        case GE_OP:
            out << " cmpq %rcx, %rax\n"
                   " movl $0, %eax\n"
                   " setge %al\n"
                   " movzbq %al, %rax\n";
            break;
        case EQ_OP:
            out << " cmpq %rcx, %rax\n"
                   " movl $0, %eax\n"
                   " sete %al\n"
                   " movzbq %al, %rax\n";
            break;
        case NE_OP:
            out << " cmpq %rcx, %rax\n"
                   " movl $0, %eax\n"
                   " setne %al\n"
                   " movzbq %al, %rax\n";
            break;
        case OR_OP:
            out << " orq %rcx, %rax\n"
                   " cmpq $0, %rax\n"
                   " movl $0, %eax\n"
                   " setne %al\n"
                   " movzbq %al, %rax\n";
            break;
        case AND_OP:
            out << " andq %rcx, %rax\n"
                   " cmpq $0, %rax\n"
                   " movl $0, %eax\n"
                   " setne %al\n"
                   " movzbq %al, %rax\n";
            break;
    }
    return 0;
}
int GenCodeVisitor::visit(AssignStatement* stm) {
    stm->rhs->accept(this);

    if (entornoClase) {
        int off = clases[nombreClase].off[stm->id];
        out << " movq %rax, " << off << "(%rdi)\n";
    } else if (memoriaGlobal.count(stm->id)) {
        out << " movq %rax, " << stm->id << "(%rip)\n";
    } else {
        out << " movq %rax, " << memoria[stm->id] << "(%rbp)\n";
    }
    return 0;
}

int GenCodeVisitor::visit(PrintStatement* stm) {
    stm->exp->accept(this);
    std::string fmt = (stm->type == "println") ? "print_fmt_ln" : "print_fmt";

    out << " movq %rsp, %rbx\n"
        << " andq $15, %rbx\n"
        << " subq %rbx, %rsp\n"
        << " movq %rax, %rsi\n"
        << " leaq " << fmt << "(%rip), %rdi\n"
        << " xor  %eax, %eax\n"
        << " call printf@PLT\n"
        << " addq %rbx, %rsp\n";

    return 0;
}

int GenCodeVisitor::visit(FunDecList* f) {
    for (auto dec : f->fundecs)
        dec->accept(this);
    return 0;
}

int GenCodeVisitor::visit(StmtList* stm) {
    for (auto s : stm->stm_list)
        s->accept(this);
    return 0;
}

int GenCodeVisitor::visit(Block* b) {
    b->vardecl->accept(this);
    b->stmdecl->accept(this);
    return 0;
}

int GenCodeVisitor::visit(IfStatement* stm) {
    int label = iflabelcont++;
    stm->condition->accept(this);
    out << " cmpq $0, %rax" << endl;
    out << " je else_" << label << endl;
    stm->then->accept(this);
    out << " jmp endif_" << label << endl;
    out << " else_" << label << ":" << endl;
    if (stm->els)
        stm->els->accept(this);
    out << "endif_" << label << ":" << endl;
    return 0;
}

int GenCodeVisitor::visit(WhileStatement* stm) {
    int label = whilelabelcont++;
    out << "while_" << label << ":" << endl;
    stm->condition->accept(this);
    out << " cmpq $0, %rax" << endl;
    out << " je endwhile_" << label << endl;
    stm->block->accept(this);
    out << " jmp while_" << label << endl;
    out << "endwhile_" << label << ":" << endl;
    return 0;
}

int GenCodeVisitor::visit(BoolExp* exp) {
    out << " movq $" << exp->value << ", %rax" << endl;
    return 0;
}

int GenCodeVisitor::visit(ReturnStatement* stm) {
    stm->ret->accept(this);
    out << " jmp .end_" << nombreFuncion << endl;
    return 0;
}

int GenCodeVisitor::visit(FCallExp* exp) {
    if (exp->is_class) {
        vector<std::string> argRegs = {"%rsi", "%rdx", "%rcx", "%r8", "%r9"};
        int size = clases[exp->nombre].size;
        out << " subq $" << size << ", %rsp" << endl;
        out << " leaq 0(%rsp), %rdi" << endl;
        size = exp->argumentos.size();
        for (int i = 0; i < size; i++) {
            exp->argumentos[i]->accept(this);
            out << " mov %rax, " << argRegs[i] << endl;
        }
        out << "call " << exp->nombre << "$ctor" << endl;
        out << " movq %rdi,"
            << "%rax" << endl;
        offset -= 8;
    } else {
        vector<std::string> argRegs = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
        int size = exp->argumentos.size();
        for (int i = 0; i < size; i++) {
            exp->argumentos[i]->accept(this);
            out << " mov %rax, " << argRegs[i] << endl;
        }
        out << "call " << exp->nombre << endl;
    }

    return 0;
}

int GenCodeVisitor::visit(UnaryExp* exp) {
    return 0;
}

int GenCodeVisitor::visit(ForStatement* stm) {
    int label = forlabelcont++;
    stm->begin->accept(this);
    out << "for_" << label << ":" << endl;
    stm->end->accept(this);
    out << " cmpq $0, %rax" << endl;
    out << " je endfor_" << label << endl;
    stm->block->accept(this);
    stm->step->accept(this);
    out << " jmp for_" << label << endl;
    out << " endfor_" << label << ":" << endl;
    return 0;
}

int GenCodeVisitor::visit(ParamList* pl) {
    return 0;
}

int GenCodeVisitor::visit(Param* p) {
    return 0;
}

int GenCodeVisitor::visit(FCallStm* stm) {
    vector<std::string> argRegs = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    int size = stm->argumentos.size();
    for (int i = 0; i < size; i++) {
        stm->argumentos[i]->accept(this);
        out << " mov %rax, " << argRegs[i] << endl;
    }
    out << "call " << stm->nombre << endl;
    return 0;
}

int GenCodeVisitor::visit(NumberExp* exp) {
    out << " movq $" << exp->value << ", %rax" << endl;
    return 0;
}

int GenCodeVisitor::visit(ClassAccessor* ca) {
    int local_offset = memoria[ca->object];
    string nombreClase = tipoClase[ca->object];
    cout << nombreClase << endl;
    int parameter_offset = clases[nombreClase].off[ca->parameter];
    out << " movq " << local_offset << "(%rbp),"
        << "%rax" << endl;
    out << " movq " << parameter_offset << "(%rax), %rax" << endl;
    return 0;
}
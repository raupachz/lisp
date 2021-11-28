
#ifndef lisp_h
#define lisp_h

#define LASSERT(args, cond, fmt, ...) \
  if (!(cond)) { \
    lval* err = lval_err(fmt, ##__VA_ARGS__); \
    lval_del(args); \
    return err; \
  }

#define LASSERT_TYPE(func, args, index, expect) \
    LASSERT(args, args->cell[index]->type == expect, \
    "Function '%s' passed incorrect type for argument %i. Got %s, Expected %s.", \
    func, index, ltype_name(args->cell[index]->type), ltype_name(expect))

#define LASSERT_NUM(func, args, num) \
    LASSERT(args, args->count == num, \
    "Funcion '%s' passed incorrect number of arguments. Got %i, Expected %i.", \
    func, args->count, num)

struct lval;
struct lenv;

typedef struct lval lval;
typedef struct lenv lenv;
typedef lval*(*lbuiltin)(lenv*, lval*);

enum {
    LVAL_ERR,
    LVAL_NUM, 
    LVAL_SYM,
    LVAL_STR,
    LVAL_FUN,
    LVAL_SEXPR,
    LVAL_QEXPR
};

struct lval {
    int type;
    /* Basic */
    long num;
    char* err;
    char* sym;
    char* str;
    /* Function */
    lbuiltin builtin;
    lenv* env;
    lval* formals;
    lval* body;
    /* Expression */
    int count;
    lval** cell;
};

struct lenv {
    lenv* parent;
    int count;
    char** syms;
    lval** vals;
};


/* These functions manage the environment */
lenv* lenv_new(void);
lenv* lenv_copy(lenv* e);
void lenv_del(lenv* e);
void lenv_put(lenv* e, lval* k, lval* v);
lval* lenv_get(lenv* e, lval* k);
void lenv_def(lenv* e, lval* k, lval* v);

/* Constructors for creating l-values */
lval* lval_num(long n);
lval* lval_err(char* fmt, ...);
lval* lval_sym(char *sym);
lval* lval_fun(lbuiltin func);
lval* lval_sexpr();
lval* lval_qexpr();
lval* lval_lambda(lval* formals, lval* body);

/* Deletes an L-value */
void lval_del(lval* v);

/* Utility functions */
void lval_expr_print(lval* v, char open, char close);
void lval_print(lval* v);
void lval_println(lval* v);
char* ltype_name(int t);

lval* lval_add(lval* v, lval* x);
lval* lval_read_num(mpc_ast_t* t);
lval* lval_read(mpc_ast_t* t);
lval* lval_pop(lval* v, int i);
lval* lval_take(lval* v, int i);
lval* lval_eval(lenv* e, lval* v);
lval* lval_join(lval* x, lval* y);
lval* lval_copy(lval* v);
lval* lval_call(lenv* e, lval* f, lval* a);
lval* lval_eval_sexpr(lenv* e, lval* v);

lval* builtin_op(lenv* e, lval* a, char* op);
lval* builtin_add(lenv* e, lval* a);
lval* builtin_sub(lenv* e, lval* a);
lval* builtin_mul(lenv* e, lval* a);
lval* builtin_div(lenv* e, lval* a);
lval* builtin_head(lenv* e, lval* a);
lval* builtin_tail(lenv* e, lval* a);
lval* builtin_list(lenv* e, lval* a);
lval* builtin_lambda(lenv* e, lval *a);
lval* builtin_var(lenv* e, lval* a, char* func);
lval* builtin_eval(lenv* e, lval* a);
lval* builtin_join(lenv* e, lval* a);
lval* builtin_gt(lenv* e, lval* a);
lval* builtin_lt(lenv* e, lval* a);
lval* builtin_ge(lenv* e, lval* a);
lval* builtin_le(lenv* e, lval* a);
lval* builtin_eq(lenv* e, lval* a);
lval* builtin_ne(lenv* e, lval* a);
lval* builtin_ord(lenv* e, lval* a, char *op);
lval* builtin_cmp(lenv* e, lval* a, char* op);
lval* builtin_if(lenv* e, lval* a);
lval* builtin_load(lenv* e, lval* a);
lval* builtin_print(lenv* e, lval* a);
lval* builtin_error(lenv* e, lval* a);
lval* builtin(lenv* e, lval* a, char* func);

#endif
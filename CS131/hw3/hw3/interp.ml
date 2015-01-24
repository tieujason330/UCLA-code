(* Name: Jason Tieu

   UID: 304047667

   Others With Whom I Discussed Things:

   Other Resources I Consulted: piazza.com
   
*)

(* EXCEPTIONS *)

(* This is a marker for places in the code that you have to fill in.
   Your completed assignment should never raise this exception. *)
exception ImplementMe of string

(* This exception is thrown when a type error occurs during evaluation
   (e.g., attempting to invoke something that's not a function).
*)
exception DynamicTypeError

(* This exception is thrown when pattern matching fails during evaluation. *)  
exception MatchFailure

(* EVALUATION *)

(* See if a value matches a given pattern.  If there is a match, return
   an environment for any name bindings in the pattern.  If there is not
   a match, raise the MatchFailure exception.
*)
let rec patMatch (pat:mopat) (value:movalue) : moenv =
  match (pat, value) with
      (* an integer pattern matches an integer only when they are the same constant;
	 no variables are declared in the pattern so the returned environment is empty *)
      (IntPat(i), IntVal(j)) when i=j -> Env.empty_env()
    | (BoolPat(i), BoolVal(j)) when i=j -> Env.empty_env() 
    | (WildcardPat, _) -> Env.empty_env() 
    | (VarPat(s), x) -> Env.add_binding s x (Env.empty_env()) 
    | (NilPat, x) -> if x=ListVal(NilVal) then Env.empty_env() else raise MatchFailure
    | (ConsPat(p1,p2), ListVal(ConsVal(a,b))) ->
      Env.combine_envs (patMatch p1 a) (patMatch p2 (ListVal(b))) 
    | _ -> raise MatchFailure 
   (* | _ -> raise (ImplementMe "pattern matching not implemented") *)

    
(* Evaluate an expression in the given environment and return the
   associated value.  Raise a MatchFailure if pattern matching fails.
   Raise a DynamicTypeError if any other kind of error occurs (e.g.,
   trying to add a boolean to an integer) which prevents evaluation
   from continuing.
*)

(* No longer needed for evalExpr of If statements
(* checkType - helper function to determine type of an expression *)
let checkType (x:movalue) : movalue =
  match x with
    IntVal(_) -> IntVal(1)
  | BoolVal(_) -> BoolVal(true)
  | ListVal(_) -> ListVal(NilVal)
  | _ -> raise MatchFailure
*)

(* listMatchHelper - helper function to find the correct expression that matches in the
   (mopat * moexpr) list. Also returns an env. Raises MatchFailure if there are no matches *)
let rec listMatchHelper (mv:movalue) (ml:(mopat * moexpr) list) : (moenv * moexpr) = 
  match (mv, ml) with
    (_, []) -> raise MatchFailure
  | (v, (pat,expr)::rest) ->
    try
      ((patMatch pat v), expr)
    with
      MatchFailure -> listMatchHelper v rest

let rec evalExpr (e:moexpr) (env:moenv) : movalue =
  match e with
      (* an integer constant evaluates to itself *)
    IntConst(i) -> IntVal(i)
 (* a bool constant evaluates to itself *)
  | BoolConst(i) -> BoolVal(i)
 (* [] evaluates to [] *)
  | Nil -> ListVal(NilVal)
 (* look up variable name in environment *)
  | Var(i) ->
    (try 
      (Env.lookup i env)
     with
       Env.NotBound -> raise (DynamicTypeError))
 (* operation between two operands, has to be int for +,-,*,=,> and for :: right-side must be list *)
  | BinOp(e1, op, e2) -> 
    let v1 = evalExpr e1 env in
    let v2 = evalExpr e2 env in
    (match (v1,op,v2) with
      | (IntVal(x), Plus, IntVal(y)) -> IntVal(x+y) 
      | (IntVal(x), Minus, IntVal(y)) -> IntVal(x-y) 
      | (IntVal(x), Times, IntVal(y)) -> IntVal(x*y) 
      | (IntVal(x), Eq, IntVal(y)) -> BoolVal(x=y) 
      | (IntVal(x), Gt, IntVal(y)) -> BoolVal(x>y)
     (* | (IntVal(x), _, IntVal(y)) -> raise DynamicTypeError (*required?*) *)
      | (_, Cons, ListVal(y)) -> ListVal(ConsVal(v1, y)) 
     (* | IntVal(x),_,_ -> raise DynamicTypeError (* 5+a for nondeclared a *)
      | _,_,IntVal(y) -> raise DynamicTypeError (* a+5 for nondeclared a *)
     *)
      | _,_,_ -> raise (DynamicTypeError)
    )
 (* negate int values *)
  | Negate(e) -> 
    let v = evalExpr e env in
    (match v with
      IntVal(x) -> IntVal(-x)
    | _ -> raise (DynamicTypeError))

 (* if condition then then-statement else else-statement *)
  | If(e1, e2, e3) ->
    let v1 = evalExpr e1 env in
    (match v1 with 
      BoolVal(x) -> 
	if x=true 
        then evalExpr e2 env
	else evalExpr e3 env
    | _ -> raise DynamicTypeError)
(*
  (*  let v2 = evalExpr e2 env in
    let v3 = evalExpr e3 env in *)
    (if (checkType v1) = BoolVal(true)
     then
(*
	let v2 = evalExpr e2 env in
        let v3 = evalExpr e3 env in
	let c2 = (checkType v2) in 
	let c3 = (checkType v3) in
	   (if (c2=c3) 
	    then
*) 
	       (match v1 with
		 BoolVal(x) -> if x=true then evalExpr e2 env else evalExpr e3 env)
	   (* else raise DynamicTypeError (* then and else must have same type *) *)
     else raise (DynamicTypeError)) (* condition needs to be a bool *) 
*)

  | Function(mp,me) -> FunctionVal(None,mp,me,env)

  | FunctionCall(e1,e2) -> 
    let f = evalExpr e1 env in
    let arg = evalExpr e2 env in
    (match f with
      FunctionVal(s,mp,me,menv) ->
	(match s with
	  (* None is non-recursive case *)
	  None -> (evalExpr me (Env.combine_envs env (Env.combine_envs menv (patMatch mp arg))))
	  (* function w/ name is recursive case *)
	| _ -> (evalExpr me (Env.combine_envs env (patMatch mp arg))))
    | _ -> raise MatchFailure
    )
				
  | Match(e, cases) -> 
    let value = evalExpr e env in
    let (evv,exx) = (listMatchHelper value cases) in
        evalExpr exx (Env.combine_envs env evv)

  | _ -> raise MatchFailure
 (* | _ -> raise (ImplementMe "expression evaluation not implemented") *)


(* Evaluate a declaration in the given environment.  Evaluation
   returns the name of the variable declared (if any) by the
   declaration along with the value of the declaration's expression.
*)
let rec evalDecl (d:modecl) (env:moenv) : moresult =
  match d with
    Expr(e) -> (None, evalExpr e env)
  | Let(x,e) -> (Some x, evalExpr e env)
  | LetRec(s,mp,me) -> (Some s, FunctionVal(Some s, mp, me, env))
  | _ -> raise MatchFailure
 (* | _ -> raise (ImplementMe "let and let rec not implemented")*)


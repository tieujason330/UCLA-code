(* Name:Jason Tieu

   UID:304047667

   Others With Whom I Discussed Things:

   Other Resources I Consulted: ocaml.org/learn/tutorials/
                                piazza.com
   
*)

(* Problem 1a
   doubleAllPos : int list -> int list *)
let doubleAllPos (l : int list) : (int list) =
  List.map(fun x -> if (x>0) then 2*x else x) l

(* Problem 1b
   unzip : ('a * 'b) list -> 'a list * 'b list *)
let unzip (l : ('a * 'b) list) : ('a list * 'b list) = 
  List.fold_right(fun (a,b) (x,y) -> (a::x, b::y)) l ([],[])

(* Problem 1c
   encode : 'a list -> (int * 'a) list *)

(* Helper function to put consecutive duplicates into sublists.
   arguments currList is for current sublist, and list of sublists  
   encodeHelper : 'a list -> 'a list list *)
let encodeHelper (l : 'a list) : ('a list list) = 
  let rec func lst currList allList  =
    match lst with
      [] -> []
    | [a] -> (a::currList)::allList
    | a::(b::_ as rest) -> 
      if a=b then func rest (a::currList) allList
      else func rest [] ((a::currList)::allList) in
  List.rev (func l [] [])

let encode (l : 'a list) : (int * 'a) list = 
 List.map (fun l -> (List.length l, List.hd l)) (encodeHelper l)

(* Problem 1d
   intOfDigits : int list -> int *)
let intOfDigits (l : int list) : int = 
  List.fold_left(fun recursiveResult x -> 10*recursiveResult + x) 0 l

(* Problem 2a
   map2 : ('a -> 'b -> 'c) -> 'a list -> 'b list -> 'c list *)
let rec map2 (f : 'a -> 'b -> 'c) (l1 : 'a list) (l2 : 'b list) : 'c list = 
  match (l1,l2) with
    [],[] -> []
  | x::xs,y::ys -> (f x y)::(map2 f xs ys)
  | _,_ -> invalid_arg "map2 requires lists with same size."

(* Problem 2b
   zip : 'a list * 'b list -> ('a * 'b) list *)
let zip ((l1 : 'a list),(l2 : 'b list)) : ('a * 'b) list = 
  map2 (fun x y -> (x,y)) l1 l2
 
(* Problem 2c
   foldn : (int -> 'a -> 'a) -> int -> 'a -> 'a *)
let rec foldn (f : int -> 'a -> 'a) (n : int) (b : 'a) : 'a = 
  match n with
      0 -> b
    | _ -> f n (foldn f (n-1) b)

(* Problem 2d
   clone : 'a * int -> 'a list *)
let clone ((e : 'a),(n : int)) : 'a list = 
  foldn (fun n x -> e::x) n []

(* Problem 3a
   empty1: unit -> ('a * 'b) list
   put1: 'a -> 'b -> ('a * 'b) list -> ('a * 'b) list
   get1: 'a -> ('a * 'b) list -> 'b option
*)  
let empty1 (() : unit) : ('a * 'b) list =
  match () with
    _ -> []

let put1 (k : 'a) (v : 'b) (l : ('a * 'b) list) : ('a * 'b) list = 
  match (k,v) with
    (k,v) -> (k,v)::l

let rec get1 (k : 'a) (l : ('a * 'b) list) : 'b option =
  match l with
    [] -> None
  |(a,b)::rest -> if a = k then Some b else get1 k rest 

(* Problem 3b
   empty2: unit -> ('a,'b) dict2
   put2: 'a -> 'b -> ('a,'b) dict2 -> ('a,'b) dict2
   get2: 'a -> ('a,'b) dict2 -> 'b option
*)  
    
type ('a,'b) dict2 = Empty | Entry of 'a * 'b * ('a,'b) dict2

let empty2 (() : unit) : ('a,'b) dict2 = 
  match () with
    _ -> Empty

(* I decided to mattern match with the wildcard only since
   the function is using currying, so each argument would 
   have to be inputted (there would be no blanks for key or
   value. *)
let put2 (k : 'a) (v : 'b) (d2 : ('a,'b) dict2) : ('a,'b) dict2  =
  match (k,v) with
    _,_ -> Entry (k,v,d2)

let rec get2 (k : 'a) (d2 : ('a,'b) dict2) : 'b option =
  match d2 with
    Empty -> None
  | Entry (a,b,d) -> if a = k then Some b else get2 k d
	
(* Problem 3c
   empty3: unit -> ('a,'b) dict3
   put3: 'a -> 'b -> ('a,'b) dict3 -> ('a,'b) dict3
   get3: 'a -> ('a,'b) dict3 -> 'b option
*)  

type ('a,'b) dict3 = ('a -> 'b option)

let empty3 (() : unit) : ('a,'b) dict3 = 
  fun s ->
    match s with
      _ -> None

(* put3 basically maps k to v, and everything else to whatever the
   dict3 dictionary currently has. Had to use wild card since if
   I pattern matched directly with k, all values would always give 
   v for the new dictionary. *)
let put3 (k : 'a) (v : 'b) (d3 : ('a,'b) dict3) : ('a,'b) dict3 = 
  fun s ->
    match s with
      | _ when s=k -> Some v 
      | _ -> d3 s

(* get3 makes the dict3 dictionary take k as an argument and returns 
   a 'b option value*)
let get3 (k : 'a) (d3 : ('a,'b) dict3) : 'b option = 
  d3 k
 

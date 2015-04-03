F.evalAST = function(ast) {
  //throw new TODO("You're supposed to write your own evaluator and hook it up to this method.");
  this.emptyEnv = new Env(undefined, undefined, undefined);
  return this.emptyEnv.eval(ast); //, new this.emptyEnv);
};
/*
function ev(ast){ //, ENV){
  //check if primitive value
    //number, boolean, null
  if(typeof ast === "number" || typeof ast === "boolean" || ast === null)
    return ast;
  else{
    var tag = ast[0];
    var args = ast.slice(1); //gives new array that starts with index of 1
    
    //args.push(ENV);
    return impls[tag].apply(undefined,args);
  }
  //else
    throw new TODO("You're supposed to write your own evaluator and hook it up to this method.");
}
*/
//check if primitive
function isPrimitive(x){
  if(typeof x === "number" || typeof x === "boolean" || x === null )
    return true
  return false;
}

//Environment
function Env(name, value, p_env) {
  this.name = name;
  this.value = value;
  this.p_env = p_env;
}

//this.emptyEnv = new Env(undefined, undefined, undefined);

Env.prototype.eval = 
  function(ast) {
    if(isPrimitive(ast)){
      return ast;
    }
    else {
      var tag = ast[0];
      var args = ast.slice(1);//gives new array that starts with index of 1
      return this.impls[tag].apply(this, args);
    }
  }
Env.prototype.lookup = 
  function(name) {
    if (this.name === name)
      return this.value;
    else if (this.p_env) { //checks if is not null/undefined
      return this.p_env.lookup(name);
    }
    else
      //return undefined;
      throw new Error(name + " does not exist in the environment.");
  };

Env.prototype.env_lookup =
  function(name) {
    if(this.name === name)
      return this;
    else if(this.p_env) {
      return this.p_env.env_lookup(name);
    }
    else
      throw new Error("There is no environment with " + name + " in it.");
  }

//function checks to see if args are numbers
Env.prototype.args_not_num = 
  function args(x,y) {
    if(typeof this.eval(x) !== "number")
     throw new Error("argument type isn't a number: " + this.eval(x));
    else if(typeof this.eval(y) !== "number")
     throw new Error("argument type isn't a number: " + this.eval(y));
  };

Env.prototype.print =
  function print(){
    if(this.p_env != undefined)
      console.log("name: " + this.name + " / value: " + this.value + " / parent: " + this.p_env.print());
    else
      console.log("name: " + this.name + " / value: " + this.value + " / parent: " + this.p_env);
  };   
/*
//function checks to see if args match
function args_no_match(x,y) {
  if(typeof x !== typeof y)
    throw new Error( (typeof x) +"  doesn't match " + (typeof y));
}
*/
/*
//function checks to see if args are boolean
Env.prototype.args_not_boolean = 
  function(x,y){
    if(typeof this.eval(x) !== "boolean")
      throw new Error("argument type isn't boolean: " + x);
    if(typeof this.eval(y) !== "boolean")
      throw new Error("argument type isn't boolean: " + y);
  };
*/

Env.prototype.impls = {
  "id": 
    function(x) {
      //console.log("in id");
      //console.log("this: " + this.print());
      //console.log("parent: " + this.p_env.print());
      return this.lookup(x);
    },
  "fun": 
    function(args, exp){
      //console.log("in fun");
      var clo = ['closure', args, exp, this];
      //console.log(clo.slice(1));
      //return this.eval(clo);
      return clo;
    },
  "call":
    function(){
      //console.log("in call");
      var call_args = Array.prototype.slice.call(arguments); //turns arguments into array
      var func_name = call_args[0]; //func name or ambiguous function
      //console.log("func_name = " + func_name);
      //console.log("func_name[0] = " + func_name[0]);
      var func;
      //func_name is name of function
      if(func_name[0] == 'id'){
        //console.log("is fun name");
        func = this.lookup(func_name[1]); //function itself from func name
      }
      else{
        //ambiguous functions
        func = this.eval(func_name);
      }
 
      var args = call_args.slice(1); //rest of array is arguments for function
      var args_eval = args.map(this.eval, this);
      //make sure there are correct number of arguments
      // ex: (fun x y -> x) 1 2 3 ===> should throw error
      //console.log("*******");
      //console.log("args.length = " + args.length);
      //console.log("args = " + args);
      //console.log("args_eval.length = " + args_eval.length);
      //console.log("func_name = " + func_name[1]);
      //console.log("func = " + func)
      //console.log("func args = " + func[1]);
      //console.log("func env = " + func[3].print());
      //console.log("func[1].length = " + func[1].length);
      //console.log("*********");
      //console.log("args[0] = " + args[0]);
      //console.log("eval args[0] = " + this.eval(args[0]));
      //console.log("args_eval[0] = " + args_eval[0]);

      //first arg
      var subEnv = new Env(func[1][0], args_eval[0], func[3]);
      var index = 1;

      if(args_eval.length > func[1].length)
        throw new Error("too many arguments");
      
      //equal number of arguments given and taken by function
      else if(args_eval.length == func[1].length){
        //must create new environments with arguments added to them
        //var subEnv = new Env(func[1][0], args_eval[0], this); //THIS ONE WRONG
        //parent_env is from closure
        //var subEnv = new Env(func[1][0], args_eval[0], func[3]);
        //console.log("this env : " + current_env.toString());

        if(args_eval.length > 1){
          for(var i = 1; i < args_eval.length; i++)
            subEnv = new Env(func[1][i], args_eval[i], subEnv);
        }
      }

      //if less arguments given than function takes ==> currying
      //should return another function
      else{
        if(args_eval.length > 1){
          for(i = 1; i < args_eval.length; i++){
            subEnv = new Env(func[1][i], args_eval[i], subEnv);
          }
          //console.log("i = " + i);
          index = i;
        }
        var newfunc = [func[0], func[1].slice(index), func[2], subEnv];
        return newfunc;
      }
      
      return subEnv.eval(func);
    },
  "closure":
    function(args, exp) {
      //throw new Error("in closure");
      //console.log("in closure");
      return this.eval(exp);
    },
  "let": 
    function(name,e1, e2){
      /*
      //old let
      v1 = this.eval(e1);
      var subEnv = new Env(name, v1, this);
      return subEnv.eval(e2);
      */
      
      //new let
      var newEnv = new Env(name, e1, this); //save the function w/o evaluating
      var v1 = newEnv.eval(e1); //closure with f in env
      newEnv.value = v1; //replace function with closure
      return newEnv.eval(e2);
    },
  //if statement
  "if":
    function(x,y,z) {
      if(typeof this.eval(x) === "boolean")
      {
        if(this.eval(x) === true)
          return this.eval(y);
        else
          return this.eval(z);
      }
      throw new Error("conditional statement isn't a boolean")
    },

  //operators
  "+": 
    function(x,y) { 
      this.args_not_num(x,y);
      return this.eval(x) + this.eval(y);
    },
  "-": 
    function(x,y) { 
      this.args_not_num(x,y);
      return this.eval(x) - this.eval(y);
    },
  "*": 
    function(x,y) { 
      this.args_not_num(x,y);
      return this.eval(x) * this.eval(y);
    },
  "/": 
    function(x,y) { 
      this.args_not_num(x,y);
      if(this.eval(y) == 0) throw new Error("cannot divide by 0.");
      return this.eval(x) / this.eval(y);
    },
  "%": 
    function(x,y) { 
      this.args_not_num(x,y);
      return this.eval(x) % this.eval(y);
    },
  "=": 
    function(x,y) { 
      return this.eval(x) === this.eval(y);
    },
  "!=": 
    function(x,y) { 
      return this.eval(x) !== this.eval(y);
    },
  "<": 
    function(x,y) { 
      this.args_not_num(x,y);
      return this.eval(x) < this.eval(y);
    },
  ">": 
    function(x,y) { 
      this.args_not_num(x,y);
      return this.eval(x) > this.eval(y);
    },
  "and": 
    function(x,y) {
      //short circuit
      var exp1 = this.eval(x);
      if(typeof exp1 === "boolean"){
        if(exp1 == false)
          return exp1;
        else{
          var exp2 = this.eval(y);
          if(typeof(exp2) === "boolean")
            return exp1 && exp2;
          else
            throw new Error("arguments need to be boolean.");
        }
      }
    },
  "or": 
    function(x,y) {
      //short circuit
      var exp1 = this.eval(x);
      if(typeof exp1 === "boolean"){
        if(exp1 == true)
          return exp1;
        else{
          var exp2 = this.eval(y);
          if(typeof(exp2) === "boolean")
            return exp1 || exp2;
          else
            throw new Error("arguments need to be boolean.");
        }
      }
    },

    //homework 2
    
    "cons":
      function(e1,e2){
        var new_cons = ['cons', this.eval(e1), this.eval(e2)];
        return new_cons;
      },
     
    "match":
      function(){
        var match_args = Array.prototype.slice.call(arguments);
        var e = match_args[0];
        var args = match_args.slice(1);

        //check there's an exp for every pattern
        if(args.length % 2 != 0){
          throw new Error("Every pattern needs an expression.");
        }

        var pats = new Array();
        var exps = new Array();


        for(var i = 0; i+1 < args.length; i=i+2){
          pats.push(args[i]);
          exps.push(args[i+1]);
        }

        var E = this.eval(e);
        
        for(var i = 0; i < pats.length; i++){
  
          //wildcard
          if(pats[i] == '_'){
            return this.eval(exps[i]);
          }
          //null
          if (pats[i] == null) {
            if (pats[i] == E) {
              return this.eval(exps[i]);
            }
            else {
              continue; //continue to next pattern so if's don't look at null
            }
          }
          //variable
          else if(pats[i][0] == 'id'){
            var subEnv = new Env(pats[i][1], E, this);
            return subEnv.eval(exps[i]);
          }
          //same as e
          else if(pats[i] === E){
            return this.eval(exps[i]);
          }
          //cons
          else if(pats[i][0] === 'cons') {// && E[0] == 'cons'){

            var subEnv = matchConsHelper(pats[i], E, this);


            if (subEnv != undefined && subEnv.name != 'mismatch') {
              return subEnv.eval(exps[i]);
            }
            else {
              //console.log("mismatch");
              if(i == pats.length-1){
                //console.log("throw");
                throw new Error("Match failure");
              }
              else {
                //console.log("test");
                continue;
              }
            }
          }
          //at last pattern and doesn't match
          else if(i == pats.length-1){
            throw new Error("Match failure");
          }
        }
         
      },
     
    "set":
      function(name, exp){
        var env_with_name = this.env_lookup(name);
        env_with_name.value = this.eval(exp); //set value
        return this.lookup(name);
      },

    "seq":
      function(e1, e2){
        this.eval(e1);
        return this.eval(e2);
      },

    "listComp":
      function(e, x, e_list, e_pred){
        return listCompHelper(e, x, e_list, e_pred, this);
      },
      
    "delay":
      function(e){
        return ['closure', [], e, this];
      },

    "force":
      function(e){
        var closure = this.eval(e); //should be a closure ['closure', arg, exp, env]
        var env = closure[3];
        return env.eval(closure); //evaluate expression in terms of it's env
      }
};

//listComp recursive helper
var listCompHelper = function(e, x, e_list, e_pred, env){
  var origList = env.eval(e_list);
  var subEnv = new Env(x, undefined, env);
  
  //base case
  if(origList == null)
    return origList;
  //one item in list
  if (origList[1] == undefined) {
    subEnv.value = env.eval(origList);
    return ['cons', subEnv.eval(e), null];
  }
  //primitive
  if (typeof(origList[1]) === 'number' || typeof(origList[1]) === 'boolean') {
    subEnv.value = env.eval(origList[1]);

    if(e_pred != undefined){
      if(subEnv.eval(e_pred) == false){
        return listCompHelper(e, x, origList[2], e_pred, subEnv);
      }
    }
    //either no e_pred or e_pred does not apply
    return ['cons', subEnv.eval(e), listCompHelper(e, x, origList[2], e_pred, subEnv)];
  }
  //:: operator involved
  else {
    subEnv.value = env.eval(origList[1][1]); //[cons, V, x]
    var newList = ['cons', origList[1][2], origList[2]];
    return ['cons', subEnv.eval(e), listCompHelper(e, x, newList, e_pred, subEnv)];
  }

  
};

//helper function to match the initial ['cons', x, y]
//if elements are not cons, uses matchExp
var matchConsHelper = function(pat, E, env){

  var subEnv;

  if (pat == null || pat[1] == null && E == null) { //checks pat[1] b/c creating new cons for recursive funcs ['cons', null, null]
    return env;
  }
  
  if (E == null) {
    E = [null,null,null];
  }
  //primitive ['cons', 1, ..]
  else if (typeof(pat[1]) === 'number' || typeof(pat[1]) === 'boolean') {
    if(pat[1] != E[1]) {
      return new Env('mismatch', undefined, env);
    }
    return matchExp(pat[2], E[2], env);
  }
  
  //[x;y] - ['cons', x, ['cons', y, null] ]
  //x::y - ['cons', x, y ]
  //_::xs - ['cons', ["_"], xs]
  //[x::y] - ['cons', ['cons', x, y], null ]
  //wildcard
  else if (pat[1] == '_') {
    return matchExp(pat[2], E[2], env);
  }
    //variable ['cons', x, ..]
  else if (pat[1][0] == 'id') {
    subEnv = new Env(pat[1][1], E[1], env);
    return matchExp(pat[2],E[2], subEnv);
  }
  else if (pat[1][0] == 'cons') {
    var e1_env = matchConsHelper(pat[1], E[1], env);
    return matchConsHelper(pat[2], E[2], e1_env);
  }
};

//recursive helper function for matchConsHelper
//looks at exp that are not cons
//if cons then uses matchConsHelper
var matchExp = function(exp, E, env) {
  
  var subEnv;
  //null
    if (exp == null) {
    if (E == null){
    return env;
    }
    else {
      //console.log("here");
      return undefined;
      //return new Env('mistmatch', undefined, undefined);
    }
  }

  if (typeof(exp) === 'number' || typeof(exp) === 'boolean') {
    if (exp == E)
      return env;
    else return undefined;
  }

  if (exp[0] == 'id') {
    subEnv = new Env(exp[1], E, env);
    return subEnv;   
  }

  if (exp[0] == '_') {
    return env;
  }

  if (exp [0] == 'cons') {
    return matchConsHelper(exp, E, env);
  }
}
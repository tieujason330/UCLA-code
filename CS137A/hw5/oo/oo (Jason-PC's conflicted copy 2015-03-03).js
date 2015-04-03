//Jason Tieu
//304047667
 
var OO = {};
 
//Class Table
OO.CT = [];
 
//Project 5 Dictionaries, Maps classes to super
var ClassToSuperDict = {};
 
////////////////////////////////////////////////////
 
//Class Table Item stores Class and List of Names
function CT_item(_Class, Name) {
  this._Class = _Class;
  if (Name === undefined)
    this.Name = [];
  else
    this.Name = [Name];
}
 
////////////////////////////////////////////////////
 
//Class object
//methods start off empty
var Class = function(name ,vars, superClass) {
  this.name = name;
  this.superClass = superClass;
  this.vars = vars;
  this.methods = [];
};
 
var Class_proto = {};
 
Class.prototype = Class_proto;
 
Class.prototype.getMethod = function(n) {
  for ( var i = 0; i < this.methods.length ; i++) {
    if (n === this.methods[i].m_name) {
      return this.methods[i];
    }
  }
  /*
  //check superclass
  var superClass = OO.getClass(this.superClass);
  for ( var i = 0; i < superClass.methods.length ; i++) {
    if (n === superClass.methods[i].m_name) {
      return superClass.methods[i];
    }
  }
  */
  throw new Error("method " + n + " does not exist");
}
 
 
/////////////////////////////////////////////////////////////
 
//Method object
var Method = function(m_name, m_body) {
  this.m_name = m_name;
  this.m_body = m_body;
}
 
////////////////////////////////////////////////////////////
 
//OO function to add CT_item
//If item exists, add name to Name array
//Else add new CT_item
OO.add_CT_item = function(CT_i) {
  for (var i = 0; i < this.CT.length ; i++) {
    if ( this.CT[i]._Class.name === CT_i._Class.name && CT_i.Name !== undefined) {
      this.CT[i].Name.push(CT_i.Name);
      return;
    }
  }
  this.CT.push(CT_i);
};
 
OO.containsClass = function(name) {
  for (var i = 0; i < this.CT.length; i++) {
    if ( this.CT[i]._Class.name === name ) {
      return true;
    }
  }
  return false;
};
 
OO.getClass = function(name) {
  for (var i = 0; i < this.CT.length; i++) {
    if ( this.CT[i]._Class.name === name ) {
      return this.CT[i]._Class;
    }
  }
  //throw new Error("Class: " + name + " does not exist.");
  return null;
};
 
OO.classOf = function(x) {
  return this.getClass(x.name);
};
 
//Helper function to see if arrays share elements
function shareElements(a1, a2) {
  for (var i = 0; i < a1.length; i++) {
    for (var j = 0; j < a2.length; j++) {
      if (a1[i] === a2[i])
        return true;
    }
  }
  return false;
}
 
OO.initializeCT = function() {
  //initialize Class Table
  this.CT = [];
 
  //Object class
  var obj = new Class("Object",[], undefined);
  ClassToSuperDict["Object"] = undefined;
  var initMethod = new Method("initialize", function(_this) {});
  obj.methods.push(initMethod);
  var equalsMethod = new Method("===", function(_this, that) { return Object.is(_this, that)});
  obj.methods.push(equalsMethod);
  var inequalsMethod = new Method("!==", function(_this, that) { return !(Object.is(_this, that))});
  obj.methods.push(inequalsMethod);
  var isNumberMethod = new Method("isNumber", function(_this) {return false;});
  obj.methods.push(isNumberMethod);
  this.CT.push(new CT_item(obj, undefined));
   
  //Number class (Part II)
  var num = new Class("Number", [], "Object");
  ClassToSuperDict["Number"] = "Object";
  var isNumberMet = new Method("isNumber", function(_this) {return true;});
  num.methods.push(isNumberMet);
  var plusMethod = new Method("+", function(_this, that) { return _this + that;});
  num.methods.push(plusMethod);
  var minusMethod = new Method("-", function(_this, that) { return _this - that;});
  num.methods.push(minusMethod);
  var multMethod = new Method("*", function(_this, that) { return _this * that;});
  num.methods.push(multMethod);
  var divMethod = new Method("/", function(_this, that) { return _this / that;});
  num.methods.push(divMethod);
  var modMethod = new Method("%", function(_this, that) { return _this % that;});
  num.methods.push(modMethod);
   
  //HW5 Part 1
  var ltMethod = new Method("<", function(_this, that) { return _this < that;});
  num.methods.push(ltMethod);
  var lteMethod = new Method("<=", function(_this, that) { return _this <= that;});
  num.methods.push(lteMethod);
  var gteMethod = new Method(">=", function(_this, that) { return _this >= that;});
  num.methods.push(gteMethod);
  var gtMethod = new Method(">", function(_this, that) { return _this > that;});
  num.methods.push(gtMethod);
  this.CT.push(new CT_item(num, undefined));
 
  var nullClass = new Class("Null", [], "Object");
  ClassToSuperDict["Null"] = "Object";
  this.CT.push(new CT_item(nullClass, undefined));
   
  var booleanClass = new Class("Boolean", [], "Object");
  ClassToSuperDict["Boolean"] = "Object";
  this.CT.push(new CT_item(booleanClass, undefined));
   
  var trueClass = new Class("True", [], "Boolean");
  ClassToSuperDict["True"] = "Boolean";
  this.CT.push(new CT_item(trueClass, undefined));
   
  var falseClass = new Class("False", [], "Boolean");
  ClassToSuperDict["False"] = "Boolean";
  this.CT.push(new CT_item(falseClass, undefined));
 
  //HW5 Part III : Blocks
  //var blockClass = new Class("Block", ["args", "stmts"], undefined);
  var blockClass = new Class("Block", [["args"],["stmts"]], undefined);
  ClassToSuperDict["Block"] = undefined;
  var callMethod = new Method("call", 
    function(_this) { 
      var args = Array.prototype.slice.call(arguments,1);
      var block_args = _this.vars["args"];
      var block_stmts = _this.vars["stmts"];
 
      console.log("args = " + args);
      console.log("block_args = " + block_args);
      console.log("block_stmts = " + block_stmts);
      var func_body = '';
 
      for(var i = 0; i < block_stmts.length ; i++) {
        if ( i !== block_stmts.length - 1) {
          func_body += block_stmts[i] + ";";
        }
        else {
          func_body += "return " + block_stmts[i] + ";";
        }
      }
      console.log("func_body = " + func_body);
       
      block_args = block_args.concat(func_body);
      console.log("block_args = " + block_args);
      Func = Function.apply(null, block_args);
      return Func.apply(this, args);
    });
  blockClass.methods.push(callMethod);
  this.CT.push(new CT_item(blockClass, undefined));
 
};
 
//helper function checks if all are unique
function varNamesUnique(instVarNames) {
  var curr;
  for ( var i = 0; i < instVarNames.length ; i++ ) {
    curr = instVarNames[i];
 
    for (var j = 0 ; j < instVarNames.length ; j++ ) {
      if ( curr === instVarNames[j] && (j !== i )) {
        return false;
      }
    }
  }
  return true;
}
 
OO.declareClass = function(name, superClassName, instVarNames) {
   
  if (varNamesUnique(instVarNames) === false) {
    throw new Error("Cannot declare variables with same name.");
  }
 
  var c = new Class(name, instVarNames, superClassName);
  ClassToSuperDict[name] = superClassName;
  var ct_item = new CT_item(c, undefined);
 
  if(this.containsClass(name)) {
    throw new Error("Duplicate class declaration");
  }
 
  if(this.containsClass(superClassName) === false) {
    throw new Error("Undeclared class");
  }
   
  var supClass = this.getClass(superClassName);
  if(shareElements(instVarNames, supClass.vars)) {
    throw new Error("Duplicate instance variable declaration");
  }
 
  this.add_CT_item(ct_item);
};
 
OO.declareMethod = function(className, selector, implFn) {
  var m;
  var curr_class = this.getClass(className);
  //check if method exists
  try {
    m = curr_class.getMethod(selector);
    m.m_body = implFn;
  } catch (Error) { //method doesn't exist
    m = new Method(selector, implFn);
    curr_class.methods.push(m);
  }
 
};
 
//create a dictionary and add key value pairs dynamically
//keys are var names from class
//values are from arguments
OO.instantiate = function(className /* , arg1, arg2, ... */){ 
  var c = this.getClass(className);
  var d =  Object.create(c);
  var newVars = c.vars;
  var sClass = this.getClass(d.superClass);
  if (sClass !== null)
  {
    d.superClass = this.getClass(c.superClass);
    newVars = d.superClass.vars.concat(c.vars);
   
    sClass = this.getClass(d.superClass.superClass);
    while (sClass !== null) {
      newVars = sClass.vars.concat(newVars);
      sClass = this.getClass(sClass.superClass);
      if (sClass === null) {
        break;
      }
    }
  }
  //d.superClass.vars = {};
  var instvars = Array.prototype.slice.call(arguments,1);
  console.log("*** in instantiate ***");
  console.log("instvars = " + instvars);
  d.vars = {};
  for ( var i = 0 ; i < instvars.length; i++ ) {
    d.vars[ newVars[i] ] = instvars[i];
  }
 
  return d;
};
 
OO.send = function(recv, selector /* ,arg1, arg2, ... */){
  var c;
  var m;
  var args;
  
  console.log("recv = " + recv); 
  //for Null/True/False class (HW4 Part I)
  if (recv === null) {
    c = this.getClass("Null");
  }
  else if (recv === true) {
    c = this.getClass("True"); 
  }
  else if (recv === false) {
    c = this.getClass("False");
  }
  //for Number class (Part II)
  else if (typeof recv === "number") {
    c = this.getClass("Number");
  }
  else {
    c = this.classOf(recv); 
  }
 
  try {
    m = c.getMethod(selector);
  } catch (Error) {
    while( c !== null ) {
      c = this.getClass(c.superClass);
      if (c === null){
        throw new Error("Method does not exist");
      }
      try {
        m = c.getMethod(selector);
        break;
      } catch (Error) {
        continue;
      }
    }
  }
 
  args = Array.prototype.slice.call(arguments, 2);
  args.unshift(recv); //add to beginning of args
  var res =  m.m_body.apply(c,args);
  if (res === undefined) {
    return null;
  }
  return res;
};
 
OO.superSend = function(superClassName, recv, selector /* , arg1, arg2, ... */) { 
  if ( this.containsClass(superClassName) === false ) {
    throw new Error("undeclared class");
  }
  var c = this.getClass(superClassName);
  var m = c.getMethod(selector);
  var args = Array.prototype.slice.call(arguments, 3);
  args.unshift(recv);
  return m.m_body.apply(recv, args);
};
 
OO.getInstVar = function(recv, instVarName) {
  var x = recv.vars[instVarName];
  var superClass = recv.superClass;
 
  while(x === undefined) {
    x = superClass.vars[instVarName];
     
    if (x === undefined) {
      throw new Error("Undeclared instance variable: " + instVarName);
    }
     
    superClass = superClass.superClass;
    if (superClass === null) {
      break;
    }
  }
   
  if (x === undefined) {
    x = superClass.vars[instVarName];
  }
  if (x === undefined) {
    throw new Error("Undeclared instance variable: " + instVarName);
  }
  return x;
};
 
OO.setInstVar = function(recv, instVarName, value) {
  if ( (instVarName in recv.vars) === false) {
    throw new Error("Undeclared instance variable: " + instVarName);
  }
  recv.vars[instVarName] = value;
  return value;
};
 
/////////////////////////////////////////////////////////////////////////////////
 
O.transAST = function(ast) {
  //throw new Error("You're supposed to write your own translator and hook it up to this method.");
  this.emptyEnv = new Env(undefined, undefined, undefined);
  return this.emptyEnv.transAST(ast);
};
 
////////////////////////////////////////////////////////////////////////
//Environment
function Env(name, value, p_env) {
  this.name = name;
  this.value = value;
  this.p_env = p_env;
}
 
//Keeps track of current Environment to be used in program 
Env.prototype.currentEnv = new Env(undefined, undefined, undefined);
 
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
  };
 
Env.prototype.transAST = function(astArr) {
  var js = "";
  var x;
  var subEnv = this;
  for ( x = 0 ; x < astArr.length ; x++ ) {
    if ( (x === 0) && (astArr[x] === "program") ) {
      OO.initializeCT();
    }
    else {
      js += subEnv.ev(astArr[x]) + ";";
      subEnv = subEnv.currentEnv;
    }
  }
  return js;
};
 
Env.prototype.ev = function(ast) {
  var tag = ast[0];
  var args = ast.slice(1);
  return this.act[tag].apply(this, args);
};
 
function arrayToString(arr) {
  var str = "[";
  for ( var i = 0; i < arr.length; i++ ) {
    if ( i === (arr.length - 1) ) {
      str += "\"" + arr[i] + "\"";
    }
    else {
      str += "\"" + arr[i] + "\",";  
    }
  }
  str += "]";
  return str;
}
 
function funcToString(args, body) {
  var str;
  if (args.length !== 0 ) {
    str = "function( _this," + args + ") {" + body + "}";
  }
  else {
    str = "function( _this) {" + body + "}";
  }
  return str;
}
 
function isArray(x) {
  return x.constructor === Array;
}
 
function printArray(arr) {
  var str = "[ ";
 
  for (var i = 0; i < arr.length; i++) {
    if (typeof arr[i] === 'string'){
      str += "\'" + arr[i] + "\' ";
    }
    else if ( isArray(arr[i]) ) {
      str += printArray(arr[i]);
    }
    else {
      str += arr[i];
    }
     
 
    if (i !== arr.length - 1) {
      str += ", ";
    }
  }
  str += " ]";
  return str;
}
 
Env.prototype.act = {
  "classDecl":
    function(className, superClassName, vars) {
      var vars_str = arrayToString(vars);
      var env = new Env("className_C", className, this);
      this.currentEnv = env;
 
      return "OO.declareClass(\"" + className + "\",\"" + superClassName + "\"," + vars_str + ")";
    },
  "methodDecl":
    function(className, methodName, args, body) {
      var check;
      //needs to save superclass somehow
      var env = new Env("className_M", className, this);
      this.currentEnv = env;
 
      var bod = "";
      for (var i = 0; i < body.length; i++) {
        bod += this.currentEnv.ev(body[i]);
        if (i !== (body.length - 1)) {
          bod += ";";
        }  
      }
       
      var func_str = funcToString(args, bod);
      return "OO.declareMethod(\"" + className + "\",\"" + methodName + "\"," + func_str + ")";
    },
  "varDecls":
    function(/*, args */) {
      var args = Array.prototype.slice.call(arguments);
      var subEnv = this;
      var rtn = "";
      for (var i = 0; i < args.length; i++) {
        var currDecl = args[i];
 
        var name = currDecl[0];
        var val = subEnv.ev(currDecl[1]);
        subEnv = new Env(name, val, subEnv);
        rtn = "var " + name + " = " + val + "\n";
      }
      this.currentEnv = subEnv;
      //return subEnv.lookup(name);
      return rtn;
    },
  "return":
      function(x) {
        this.currentEnv = this;
        return "return " + this.ev(x) + ";";
    },
  "setVar":
    function(x, val) {
      var env = this.env_lookup(x);
      env.value = this.ev(val);
      this.currentEnv = this;
      return this.lookup(x);
    },
  "setInstVar":
    function(x, val) {
      this.currentEnv = this;
      return "OO.setInstVar(_this,\"" + x + "\"," + this.ev(val) + ")";
    },
  "exprStmt":
    function(expr) {
      this.currentEnv = this;
      return this.ev(expr);
    },
  "null":
    function() {
      this.currentEnv = this;
      return "null";
    },
  "true":
    function() {
      this.currentEnv = this;
      return "true";
    },
  "false":
    function() {
      this.currentEnv = this;
      return "false";
    },
  "number":
    function(num) {
      this.currentEnv = this;
      return ""+num;
    },
  "string":
    function(str) {
      this.currentEnv = this;
      return "\""+ str + "\"";
    },
  "getVar":
    function(x) {
      this.currentEnv = this;
      try {
        return this.lookup(x);
      } catch (Error) { //catch for methodDecl
        return x;
      }
    },
  "getInstVar":
    function(x) {
      return "OO.getInstVar(_this,\"" + x + "\")";  
    },
  "new":
      function(className /*, args */) {
        this.currentEnv = this;
        var args = Array.prototype.slice.call(arguments, 1);
        var ev_args = [];
        for (var i = 0; i < args.length; i++) {
          ev_args.push(this.ev(args[i]));
        }
 
        if (args.length !== 0) {
          return "OO.instantiate(\"" + className + "\"," + ev_args + ")";
        }
        else {
          return "OO.instantiate(\"" + className + "\")";
        }
    },
  "this":
    function() {
      this.currentEnv = this;
      try {
        return this.lookup("this");
      } catch (Error) { //catch for methodDecl
        return "this";
      }
    },
  "send":
      function(recv, methodName /*, args */) {
        this.currentEnv = this;
        var args = Array.prototype.slice.call(arguments, 2);
        var ev_args = [];
        for (var i = 0; i < args.length; i++) {
          ev_args.push(this.ev(args[i]));
        }
 
        if (ev_args.length !== 0) {
          return "OO.send(" + this.ev(recv) + ",\"" + methodName + "\"," + ev_args + ")";
        }
        else {
          return "OO.send(" + this.ev(recv) + ",\"" + methodName + "\")";
        }
    },
  "super":
      function(methodName /*, args */) {
        this.currentEnv = this;
        var args = Array.prototype.slice.call(arguments, 1);
        var ev_args = [];
        for (var i = 0; i < args.length; i++) {
          ev_args.push(this.ev(args[i]));
        }
        //var superClassName = this.lookup("superClassName_C");
         
        var superClassName;
        try {
          superClassName = ClassToSuperDict[this.lookup("className_C")];
        } catch (Error) {
          superClassName = ClassToSuperDict[this.lookup("className_M")];
        }
 
        if (ev_args.length !== 0) {
          return "OO.superSend(\"" + superClassName + "\", _this,\'" + methodName + "\'," + ev_args + ")";
        }
        else {
          return "OO.superSend(\"" + superClassName + "\", _this,\'" + methodName + "\')";
        }
    },
  "block":
    function(args, stmts) {
      var ev_args = [];
      var ev_stmts = [];
      /*
      if (args.length !== 0) {
        for (var i = 0; i < arguments.length; i++) {
          ev_args.push((arguments[i]));
        }
      }
      */
      if (stmts.length !== 0) {
        for (var i = 0; i < stmts.length; i++) {
          ev_stmts.push(this.ev(stmts[i]));
        }
      }
      else {
        ev_stmts.push("null");
      }
      console.log("ev_stmts = " + ev_stmts);
      console.log("printArray(ev_stmts) = " + printArray(ev_stmts));
      console.log("OO.instantiate(\"" + "Block" + "\",[" + args + "],[ " +  ev_stmts + "])");
      return "OO.instantiate(\"" + "Block" + "\"," + printArray(args) + ", " +  printArray(ev_stmts) + ")";
    }
}
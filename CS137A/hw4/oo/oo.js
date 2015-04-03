//Jason Tieu
//304047667

var OO = {};

//Class Table
OO.CT = [];

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
  this.CT.push(new CT_item(num, undefined));
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
  if (this.getClass(d.superClass) !== null)
  {
    d.superClass = this.getClass(c.superClass);
    newVars = d.superClass.vars.concat(c.vars);
  }
  //d.superClass.vars = {};
  var instvars = Array.prototype.slice.call(arguments,1);
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
  
  //for Number class (Part II)
  if (typeof recv === "number") {
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
      if (c === null)
        throw new Error("Method does not exist");
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
  return m.m_body.apply(c,args);
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
  if (x === undefined) {
    x = recv.superClass.vars[instVarName];
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
//create variable for wildcard
var _ = function() { return "_" };

function when(f) {
  if (!(this instanceof when)){
        return new when(f);
   }

   this.f = f;
}

function many(x) {
   if (!(this instanceof many)){
        return new many(x);
   }

   this.x = x;
}

function match(value /* , pat1, fun1, pat2, fun2, ... */) {
  
  var v = value;
  var args = Array.prototype.slice.call(arguments, 1);
  var bindings = [];
  //check there's an exp for every pattern
  if(args.length % 2 != 0){
    throw new Error("Every pattern needs an expression.");
  }

  var pats = new Array();
  var exps = new Array();

  for (var i = 0 ; i+1 < args.length ; i=i+2){
    pats.push(args[i]);
    exps.push(args[i+1]);
  }
  
  for(var i = 0; i < pats.length; i++) {

    //if value and pattern are literals
    if ( isLiteral(v) && isLiteral(pats[i]) ) {
      if (v == pats[i]) {
        bindings = [];
        return doExpression(exps[i], bindings);
      }
    }
    //pattern is just wildcard
    else if ( pats[i] === _ ) {
      bindings = [v];
      return doExpression(exps[i], bindings);
    }
    
    //value is array and pattern is array
    //make sure both arrays are same size
    //if pattern has wildcards do bindings to pass to expression function
    //else check if arrays equal
    //need to get bindings
    else if ( isArray(v) && isArray(pats[i]) ) {
      if ( v.length === pats[i].length ) {
        if ( hasWildcard(pats[i]) || arrayHasMany(pats[i]) ) {
          bindings = createBindingsArray(v, pats[i], bindings);
          return doExpression(exps[i], bindings);
        }
        else {
          if ( arraysEqual(v, pats[i]) == true ) {
            //no bindings
            return doExpression(exps[i], bindings);
          }
        }
      }
      else {
        
        if ( arrayHasMany(pats[i]) ) {
          bindings = createBindingsArray(v, pats[i], bindings);
          //throw new Error(isArray(bindings));
          return doExpression(exps[i], bindings);
        }
        if ( i == (pats.length - 1) ) {
          throw new Error("match failed");
        }
        continue;
      }
    }
    //pattern is when(f)
    else if ( pats[i] instanceof when ) {
      var when_i = pats[i];
      if ( when_i.f(v) === true ) {
        bindings = [v];
        return doExpression(exps[i], bindings);
      }
    }
    
    else if ( i == (pats.length - 1) ) {
      throw new Error("match failed");
    }
  }
}

function createBindingsMany(val, manyElem, bindings) {
  for (var i = 0; i < val.length; i++) {
    if( manyElem === _ ) {
      bindings.push(val[i]);
      console.log("bindings = " + bindings);
    }
    else if ( manyElem instanceof when ) {
      //when = manyElem.x;
      if ( manyElem.f(val[i]) === true) {
        bindings.push(val[i]);
      }
      else {
        bindings = [bindings, []];
        break;
      }
    }
    else if ( isArray(manyElem) ) {
      for(var i = 0; i < val.length; i++) {
        bindings = createBindingsArray(val[i], manyElem, bindings);
      }
    }
    else if (manyElem instanceof many) {
      throw new Error("many must be within an array");
    }
    else if ( isLiteral(manyElem) ) {
      bindings.push([]);
    }
  }
  return bindings;
}

//function to create bindings for patterns
function createBindingsArray(val, pat, bindings) {
 //var bindings = [];
  for( var i = 0 ; i < pat.length ; i++ ) {
    if( pat[i] === _ ) {
      bindings.push(val[i]);
    }
    else if( pat[i] instanceof many) {
      bindings = createBindingsMany(val.slice(i) , pat[i].x, bindings) ;
      //val = undefined;
    }
    else if (val === undefined) {
      throw new Error("match failed.");
    }
  }
  //console.log("bindings array = " + bindings);
  return bindings;
}

//function checks if array has functions in it
//returns index if it does
//else returns false
function arrayHasFunction(a) {
  for ( var i = 0 ; i < a.length ; i++ ) {
    if ( isFunction(a[i]) )
      return i;
  }
  return false;
}
//function to determine if argument is a literal
//number, boolean, string, undefined
function isLiteral(x) {
  if(typeof(x) === "number" 
  || typeof(x) === "boolean"
  || typeof(x) === "string" 
  || typeof(x) === "undefined")
    return true;

   return false;
}
//helper function to determine what expression is
//and execute it
function doExpression(e, e_args) {

  if( isFunction(e) ) {
    if(e.length <= 1) {
      //throw new Error("in here");
      if(e_args.length > 1) {
        return e.call(this, e_args);
      }
      return e.call(this, e_args[0]);
    }
    //used try block since need to use call is arg is an array
    //apply takes an arbitrary number of args as an array
    try {
      return e.apply(this, e_args);
    }
    catch (TypeError) {
      return e.call(this, e_args);
    }
  }
  throw new Error("Expression must be a function");
}

function isOne(x) {
  return (x === 1);
}
//helper function to determine if f is a function
function isFunction(f) {
  var getType = {};
  return f && getType.toString.call(f) === '[object Function]';
}

//helpr function to determine if x is an array
function isArray(x) {
  return x.constructor === Array;
}

//helper function that checks if arrays equal
function arraysEqual(a1, a2) {
  if ( a1.length !== a2.length )
    return false;
    
  for(var i = 0; i < a1.length; i++ ) {
    if ( a1[1] !== a2[i] )
      return false;
  }
  return true;
}

//helper function that checks if wildcard exists in array
function hasWildcard (a) {
  for ( var i = 0; i < a.length; i++ ) {
    if ( a[i] === _ )
      return true;
  }
  return false;
}

//function checks if many is in array
function arrayHasMany (a) {
  for ( var i = 0; i < a.length; i++ ) {
    if ( a[i] instanceof many )
      return true;
  }
  return false;
}
// -----------------------------------------------------------------------------
// Part I: Rule.prototype.makeCopyWithFreshVarNames() and
//         {Clause, Var}.prototype.rewrite(subst)
// -----------------------------------------------------------------------------

Rule.prototype.makeCopyWithFreshVarNames = function() {
  //throw new TODO("Rule.prototype.makeCopyWithFreshVarNames not implemented");
  var cpy_head_args = this.head.args.slice(); //copy arrays by value
  var cpy_body = this.body.slice();
  
  for ( var i = 0 ; i < cpy_head_args.length ; i++ ) {
    if ( cpy_head_args[i] instanceof Var ) {
      cpy_head_args[i] = new Var(cpy_head_args[i].name + "_cpy");
    }
  }
  
  for ( var i = 0; i < cpy_body.length ; i++ ) {
    for ( var j = 0 ; j < cpy_body[i].args.length ; j++ ) {
      if ( cpy_body[i].args[j] instanceof Var) {
        cpy_body[i].args[j] = new Var(cpy_body[i].args[j].name + "_cpy");
      }
    }
  }
  var head = new Clause(this.head.name, cpy_head_args);
  return new Rule(head, cpy_body);
};

function rewriteClauseHelper(clause, subst) {
  //console.log(clause);
  if (clause.args === [] ) {
    return clause;
  }
  
  for ( var i = 0 ; i < clause.args.length ; i++ ) {
    if ( clause.args[i] instanceof Var ) {
      var cla = subst.lookup(clause.args[i].name);
      if ( cla !== undefined ) {
        clause.args[i] = cla;
      }
    }
    else {
      rewriteClauseHelper(clause.args[i], subst);
    }
  }
  return clause;
}

Clause.prototype.rewrite = function(subst) {
  //throw new TODO("Clause.prototype.rewrite not implemented");
  var sub = new Subst();
  rewriteClauseHelper(this, subst);
  return this;
};

Var.prototype.rewrite = function(subst) {
  //throw new TODO("Var.prototype.rewrite not implemented");
  var sub = new Subst();
  var key = Object.keys(s.bindings)[0];
  if (key === this.name) {
    return subst.lookup(key);
  }
  else {
    sub.bind(this.name, subst.lookup(key));
    return this;
  }
};

// -----------------------------------------------------------------------------
// Part II: Subst.prototype.unify(term1, term2)
// -----------------------------------------------------------------------------
Subst.prototype.combineBindings = function(sub){
  var sub_keys = Object.keys(sub.bindings);
  for ( var i = 0 ; i < sub_keys.length ; i++ ) {
    this.bindings[sub_keys[i]] = sub.lookup(sub_keys[i]);
  }

  return this;
};

function unifyHelper(term1, term2) {
  if ( term1 instanceof Var ) {
    if (new Subst().bind(term1.name, term2) === undefined) {
      return;
    }
    else if ( term2 instanceof Clause ) { //term2 is Clause
      return new Subst().bind(term1.name, term2);  
    }
    else { //term2 is Var
      return new Subst().bind(term1.name, term2);
    }
  }
  else { //term1 is Clause
    if ( term2 instanceof Clause ) { //term2 is Clause
      if ( term1.args.length === 0 && term2.args.length === 0 ) {
        if ( term1.name === term2.name ) {
          return new Subst();
        }
        else { 
          throw new Error("Clause-Clause different names.");
        }
      }
      else {
        if (term1.name === term2.name) {
          var sub = new Subst();
          for ( var i = 0 ; i < term1.args.length ; i++ ) {
            //console.log("i = " + i + " , sub = " + sub);
            
            var sub_temp = unifyHelper(term1.args[i], term2.args[i]);
            
            if ( Object.keys(sub_temp.bindings).length !== 0 ) {
              sub.combineBindings(sub_temp);
            }
          }
          //check keys to make sure no instances of X = a, Y = X
          var keys = Object.keys(sub.bindings);
          
          for ( var i = 0 ; i < keys.length ; i++ ) {
            var val = sub.lookup(keys[i]);
            if ( sub.lookup(val) !== undefined ) {
              sub.bindings[keys[i]] = sub.lookup(val);
            }  
          }
          return sub;
        }
        else {
          throw new Error("Clause-Clause different names.");
        }
      }
    }
    else { //term2 is Var
      return new Subst().bind(term2.name, term1);
    }
  }
}

Subst.prototype.unify = function(term1, term2) {
  //throw new TODO("Subst.prototype.unify not implemented");
  var sub = unifyHelper(term1, term2);
  this.combineBindings(sub);
  return sub;
};

// -----------------------------------------------------------------------------
// Part III: Program.prototype.solve()
// -----------------------------------------------------------------------------
function ClauseNameStruct(name, arr) {
  this.name = name; //name of query
  this.arr = arr; //rules that match that query
}

function Iterator(rules, query /*, clauseNameArr */) {
  this.rules = rules;
  this.query = query;
  //this.clauseNameArr = clauseNameArr;
}


Iterator.prototype.nextHelper = function(r, query) {
  var rule = r.makeCopyWithFreshVarNames();
  if (rule.body.length === 0) {
    var r_clause = rule.head;
    return new Subst().unify(r_clause, query);
  }
  else {
    throw new Error("TODO RULE BODY");
  }
};


Iterator.prototype.next = function() {
  var sub = new Subst();
  var noSolutions = [];
  var matchedElements = [];
  for ( var i = 0 ; i < this.query.length ; i++ ) {
    for ( var j = 0 ; j < this.rules.length ; j++ ) {
      if (this.query.length === 1) { //1 query
        try {
          sub = this.nextHelper(this.rules[j], this.query[i]);
          this.rules = this.rules.slice(1);
          return sub;
        } catch (Error) {
          continue;
        }
      }
      else { //multiple queries
        try {
          sub = this.nextHelper(this.rules[j], this.query[i]);
          noSolutions[i] = false;
          matchedElements.push(j);
          break;
        } catch (Error) {
          noSolutions[i] = true;
          continue;
        }
      }
    }
  }
  for ( var k = 0 ; k < this.rules.length ; k++ ) {
    console.log(this.rules[k]);
  }
  //console.log(this.rules);
  console.log(matchedElements);
  for ( var j = 0 ; j < matchedElements.length ; j++ ) {
    console.log("here");
    this.rules.splice(matchedElements[j],1);
  }
  if ( noSolutions.length > 1 ) {
    for ( var i = 0 ; i < noSolutions.length ; i++ ) {
      if ( noSolutions[i] === true ) {
        //this.rules = this.rules.slice(1);

        return null;
      }
    }
    //this.rules = this.rules.slice(this.rules.length);
    //this.rules = this.rules.slice(1);
    return sub;
  }

  return null;
};

/*
Program.prototype.createIteratorStructs = function() {

  var c_namesArr = [];
  for ( var i = 0 ; i < this.query.length ; i++ ) {
    var name = this.query[i].name; 
    var arr = [];

    for ( var j = 0 ; j < this.rules.length ; j++ ) {
      var rule_clause = this.rules[j].head;

      if ( rule_clause.name === name ) {
        if ( rule_clause.args.length !== 0 ) {
          arr.push(rule_clause.args);
        }
        else {
          arr.push(rule_clause.name);
        }
      }
    }
    c_namesArr.push( new ClauseNameStruct(name, arr) );
  }
  return c_namesArr;
}

Iterator.prototype.next = function () {
  
  var sub = new Subst();
  for ( var i = 0; i < this.query.length ; i++ ) {
    var name = this.query[i].name;

    if ( this.clauseNameArr.length === 0 ) {
      return null;
    }
    for ( var j = 0 ; j < this.clauseNameArr.length ; i++ ) {
      if ( name === this.clauseNameArr[i].name ) {
        if ( this.clauseNameArr[i].arr.length === 0 ) {
          return null;
        }
        else {
          try {
            sub.unify(this.clauseNameArr[i] , this.query[i]);
            return sub;
          } catch (Error) {
            continue;
          }
        }
      }
    }
  }

  return null;
}
*/
Program.prototype.solve = function() {
  //throw new TODO("Program.prototype.solve not implemented");
  //var c_namesArr = this.createIteratorStructs();
  return new Iterator(this.rules, this.query); //,c_namesArr);
};
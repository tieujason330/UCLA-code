function GoogleSuggestController(oTextbox) {
    this.cur = -1;
    this.layer = null;
    this.textbox = oTextbox;
    this.xmlHttp = new XMLHttpRequest();
    
    this.init();
}

GoogleSuggestController.prototype.autosuggest = function (oThis) {
     

     return function () 
     {   
	     if (oThis.xmlHttp.readyState == 4) 
	     {
	     		var aSuggestions = [];
				 
			       var s = oThis.xmlHttp.responseXML.getElementsByTagName('CompleteSuggestion');
				 
				
				  for (i = 0; i < s.length; i++) 
				  {
				  	var text = s[i].childNodes[0].getAttribute("data");
				
				  	aSuggestions.push(text);
				  }

				  if (aSuggestions.length > 0) 
				  {    
				  	oThis.showSuggestions(aSuggestions);
				  } 
				  else 
				  {
				  	oThis.hideSuggestions();
				  }
			 }
		 }
};

GoogleSuggestController.prototype.selectRange = function (iStart, iLength ) {

    if (this.textbox.createTextRange) {
        var oRange = this.textbox.createTextRange(); 
        oRange.moveStart("character", iStart); 
        oRange.moveEnd("character", iLength - this.textbox.value.length);      
        oRange.select();
        
    } else if (this.textbox.setSelectionRange) {
        this.textbox.setSelectionRange(iStart, iLength);
    }     

    this.textbox.focus();      
}; 

GoogleSuggestController.prototype.typeAhead = function (sSuggestion) {

    if (this.textbox.createTextRange || this.textbox.setSelectionRange){
        var iLen = this.textbox.value.length; 
        this.textbox.value = sSuggestion; 
        this.selectRange(iLen, sSuggestion.length);
    }
};

GoogleSuggestController.prototype.handleKeyUp = function (oEvent) {

    var iKeyCode = oEvent.keyCode;

    if ((iKeyCode < 32 && iKeyCode != 8) || (iKeyCode >= 33 && iKeyCode < 46) || (iKeyCode >= 112 && iKeyCode <= 123)) {
    	//ignore 
    } 
    else {
        this.requestSuggestions(); 
    }
};

GoogleSuggestController.prototype.init = function () {
    var oThis = this;
    this.textbox.onkeyup = function (oEvent) {
        if (!oEvent) {
            oEvent = window.event;
        }    
        oThis.handleKeyUp(oEvent);
    };
    
    this.textbox.onkeydown = function (oEvent) {
        if (!oEvent) {
            oEvent = window.event;
        }    
        oThis.handleKeyDown(oEvent);
    };
        
    this.textbox.onblur = function () {
        oThis.hideSuggestions();
    };
    this.createDropDown();
};

GoogleSuggestController.prototype.createDropDown = function () {

    var oThis = this;

    this.layer = document.createElement("div");
    this.layer.className = "suggestions";
    this.layer.style.visibility = "hidden";
    this.layer.style.width = this.textbox.offsetWidth;

    this.layer.onmousedown = 
    this.layer.onmouseup = 
    this.layer.onmouseover = function (oEvent) {
        oEvent = oEvent || window.event;
        oTarget = oEvent.target || oEvent.srcElement;

        if (oEvent.type == "mousedown") {
            oThis.textbox.value = oTarget.firstChild.nodeValue;
            oThis.hideSuggestions();
        } else if (oEvent.type == "mouseover") {
            oThis.highlightSuggestion(oTarget);
        } else {
            oThis.textbox.focus();
        }
    };
    
    
    document.body.appendChild(this.layer);
};

GoogleSuggestController.prototype.getLeft = function () {

    var oNode = this.textbox;
    var iLeft = 0;
    
    while(oNode.tagName != "BODY") {
        iLeft += oNode.offsetLeft;
        oNode = oNode.offsetParent;        
    }
    
    return iLeft;
};

GoogleSuggestController.prototype.getTop = function (){

    var oNode = this.textbox;
    var iTop = 0;
    
    while(oNode.tagName != "BODY") {
        iTop += oNode.offsetTop;
        oNode = oNode.offsetParent;
    }
    
    return iTop;
};

GoogleSuggestController.prototype.handleKeyDown = function (oEvent) {

    switch(oEvent.keyCode) {
        case 38: //up arrow
            this.previousSuggestion();
            break;
        case 40: //down arrow 
            this.nextSuggestion();
            break;
        case 13: //enter
            this.hideSuggestions();
            break;
    }

};



GoogleSuggestController.prototype.hideSuggestions = function () {
    this.layer.style.visibility = "hidden";
};

GoogleSuggestController.prototype.highlightSuggestion = function (oSuggestionNode) {
    
    for (var i=0; i < this.layer.childNodes.length; i++) {
        var oNode = this.layer.childNodes[i];
        if (oNode == oSuggestionNode) {
            oNode.className = "current"
        } else if (oNode.className == "current") {
            oNode.className = "";
        }
    }
};


GoogleSuggestController.prototype.nextSuggestion = function () {
    var cSuggestionNodes = this.layer.childNodes;

    if (cSuggestionNodes.length > 0 && this.cur < cSuggestionNodes.length-1) {
        var oNode = cSuggestionNodes[++this.cur];
        this.highlightSuggestion(oNode);
        this.textbox.value = oNode.firstChild.nodeValue; 
    }
};

GoogleSuggestController.prototype.requestSuggestions = function () 
{
    var sTextboxValue = this.textbox.value;
		
		if (sTextboxValue != ""){
	    var request = "suggest?q="+encodeURI(sTextboxValue);
	
	    this.xmlHttp.open("GET", request); 
			this.xmlHttp.onreadystatechange = this.autosuggest(this); 
			this.xmlHttp.send(null);
		}
		else{
			this.hideSuggestions();
		}
};

GoogleSuggestController.prototype.showSuggestions = function (aSuggestions ) {
    
    var oDiv = null;
    this.layer.innerHTML = ""; 
    
    for (var i=0; i < aSuggestions.length; i++) {
        oDiv = document.createElement("div");
        oDiv.appendChild(document.createTextNode(aSuggestions[i]));
        this.layer.appendChild(oDiv);
    }
    
    this.layer.style.left = this.getLeft() + "px";
    this.layer.style.top = (this.getTop()+this.textbox.offsetHeight) + "px";
    this.layer.style.visibility = "visible";

};

GoogleSuggestController.prototype.previousSuggestion = function () {
    var cSuggestionNodes = this.layer.childNodes;

    if (cSuggestionNodes.length > 0 && this.cur > 0) {
        var oNode = cSuggestionNodes[--this.cur];
        this.highlightSuggestion(oNode);
        this.textbox.value = oNode.firstChild.nodeValue;   
    }
};







var HTTPrequest = new XMLHttpRequest();//create a new request object
var _count	= 0;//record the times of button been pressed

window.onload = function()
{
    getTime();//display time and data in footer
}

function loadAbout()
{
    loadPage("about.htm");
    return false;
}

function loadIO()
{
    loadPage("io.htm");
//	speedGet();
    return false;
}

//异步加载页面
function loadPage(page)
{
    if(window.XMLHttpRequest)
    {
        xmlhttp = new XMLHttpRequest();
    }
    else
    {
        xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
    }

    xmlhttp.open("GET", page, true);//set the method 
    xmlhttp.setRequestHeader("Content-type",
                             "application/x-www-form-urlencoded");
    xmlhttp.send();//send the message to http server

    xmlhttp.onreadystatechange = function ()
    {
        if((xmlhttp.readyState == 4) && (xmlhttp.status == 200))
        {
            document.getElementsByClassName("main")[0].innerHTML = xmlhttp.responseText;
        }
    }
}

//set default value
function SetFormDefaults()
{
    document.iocontrol.LEDOn.checked = ls;
    document.iocontrol.speed_percent.value = sp;
}

// helper function for getting token values 
function getTokenValue(paramPage, tokenId, successFn, failFn) {
	$.ajax({
			"type": "GET",
			"url": paramPage,
			"cache": false,
			"dataType": "html"
		})
		// define callback for get request success
		.done(function(getdata, status, xhr) {
			successFn($(getdata).filter('#'+tokenId).text());
		})
		// define callback for get request success
		.fail(function(jqxhr, settings, exception) {
			failFn();
		});
		
}

function get_button1()
{
	getTokenValue('io_http_params.htm', 'button1_count_status', 
				// successFn callback, val is the token value
				function(val) 
				{ 
					document.getElementById("button1_count").value = val;
				},
				// failFn callback
				function() 
				{ 
                   _count = _count + 1;
				});	
}

function get_temperature()
{
	getTokenValue('io_http_params.htm', 'temperature_status', 
				// successFn callback, val is the token value
				function(val) 
				{ 
					document.getElementById("temperature").value = val;//set the temperature equal to val
				},
				// failFn callback
				function() 
				{ 
                   _count = _count + 1;
				});	
}

function get_humid()
{
	getTokenValue('io_http_params.htm', 'temperature_status', 
				// successFn callback, val is the token value
				function(val) 
				{ 
					document.getElementById("humidity").value = val;
				},
				// failFn callback
				function() 
				{ 
                   _count = _count + 1;
				});	
}

//var HTTPrequest = new XMLHttpRequest();
function led_on()
{
	var params = "__SL_P_U00";
	params = params +"LED_ON";//pass the turn led on message to server
	HTTPrequest.open("POST","No_content", true);
	HTTPrequest.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	HTTPrequest.onreadystatechange = function()
	{
		if (HTTPrequest.readyState==4 && HTTPrequest.status==200)
		{
		}
	}
	HTTPrequest.send(params);
}
		
function led_off()
{
	var params = "__SL_P_U01";
	params = params+"LED_OFF";
	HTTPrequest.open("POST","No_content", true);
	HTTPrequest.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	HTTPrequest.onreadystatechange = function()
	{
		if (HTTPrequest.readyState==4 && HTTPrequest.status==200)
		{
		}
	}
	HTTPrequest.send(params);
}

function led_breath_on(){
	var params = "__SL_P_U06";
	params = params+"pwm_OFF";
	HTTPrequest.open("POST","No_content", true);
	HTTPrequest.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	HTTPrequest.onreadystatechange = function()
	{
		if (HTTPrequest.readyState==4 && HTTPrequest.status==200)
		{
		}
	}
	HTTPrequest.send(params);
}

function led_breath_off(){
	var params = "__SL_P_U10";
	params = params +"pwm_OFF";
	HTTPrequest.open("POST","No_content", true);
	HTTPrequest.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	HTTPrequest.onreadystatechange = function()
	{
		if (HTTPrequest.readyState==4 && HTTPrequest.status==200)
		{
		}
	}
	HTTPrequest.send(params);
}
	
function led_toggle_on()
{
	var params = "__SL_P_U02";
	params = params +"LED_TOGGLE_ON";
	HTTPrequest.open("POST","No_content", true);
	HTTPrequest.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	HTTPrequest.onreadystatechange = function()
	{
		if (HTTPrequest.readyState==4 && HTTPrequest.status==200)
		{
		}
	}
	HTTPrequest.send(params);
}
		
function led_toggle_off()
{
	var params = "__SL_P_U03";
	params = params +"LED_TOGGLE_OFF";
	HTTPrequest.open("POST","No_content", true);
	HTTPrequest.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	HTTPrequest.onreadystatechange = function()
	{
		if (HTTPrequest.readyState==4 && HTTPrequest.status==200)
		{
		}
	}
	HTTPrequest.send(params);
}
		
function temperatureC()
{
	var params = "__SL_P_U08";
	params = params + "TEMP_IN_C";
	HTTPrequest.open("POST","No_content", true);
	HTTPrequest.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	HTTPrequest.onreadystatechange = function()
	{
		if (HTTPrequest.readyState==4 && HTTPrequest.status==200)
		{
		}
	}
	HTTPrequest.send(params);
    get_temperature();
}

function humid()
{
	var params = "__SL_P_U09";
	params = params + "TEMP_IN_F";
	HTTPrequest.open("POST","No_content", true);
	HTTPrequest.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	HTTPrequest.onreadystatechange = function()
	{
		if (HTTPrequest.readyState==4 && HTTPrequest.status==200)
		{
		}
	}
	HTTPrequest.send(params);
    get_humid();
}

//set the speed of LED toggling or breathing	
function speedSet()
{
	var params = "__SL_P_U04";
    var speed_txt = document.getElementById("speed_percent").value;//get the value of user's input
	params = params+ "_" + speed_txt + "END";
	HTTPrequest.open("POST","No_content", true);
	HTTPrequest.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	HTTPrequest.onreadystatechange = function()
	{
		if (HTTPrequest.readyState==4 && HTTPrequest.status==200)
		{
		}
	}
	HTTPrequest.send(params);
}


		
function clear_button1()
{
	var params = "__SL_P_U05";
	params = params+ "CLEAR BUTTON 1 COUNT";
    HTTPrequest.open("POST","No_content", true);
	HTTPrequest.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	HTTPrequest.onreadystatechange = function()
	{
		if (HTTPrequest.readyState==4 && HTTPrequest.status==200)
		{
		}
	}
	HTTPrequest.send(params);
	document.getElementById("button1_count").value = "0";
}

//get the value of LED toggling or breathing speed	
function speedGet()
{
	var status = "__SL_G_U04";
	var speed = status.slice(8,9);
	if(speed == "0")
	{
		speed = status.slice(9,10);
	}
	else
	{
		speed = status.slice(8,10);
	}
	
	if(status.indexOf("LED1_ON") > -1)
	{
		document.getElementById("ledstate").value="ON";
		document.getElementById("ledstate").innerHTML = "ON";
	}
	else
	{
		document.getElementById("ledstate").value="OFF";
		document.getElementById("ledstate").innerHTML = "OFF";
	}
	document.getElementById("current_speed").innerHTML = "<div>" + speed +"%" + "</div>";
}

function getTime(){
	var myDate=new Date();
	document.getElementById("time").innerHTML=myDate.toLocaleString();
}

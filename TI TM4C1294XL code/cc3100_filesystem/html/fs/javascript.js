<!-- Copyright (c) 2013-2014 Texas Instruments Incorporated.  All rights reserved. -->
var HTTPrequest = new XMLHttpRequest();
window.onload = function()
{
    document.getElementById('about').onclick = loadAbout;
    document.getElementById('overview').onclick = loadOverview;
    document.getElementById('block').onclick = loadBlock;
    document.getElementById('io_http').onclick = loadIOHttp;
}

function loadAbout()
{
    loadPage("about.htm");
    return false;
}

function loadOverview()
{
    loadPage("overview.htm");
    return false;
}

function loadBlock()
{
    loadPage("block.htm");
    return false;
}

function loadIOHttp()
{
    loadPage("io_http.htm");
//	speedGet();
    return false;
}

function SetFormDefaults()
{
    document.iocontrol.LEDOn.checked = ls;
    document.iocontrol.speed_percent.value = sp;
}


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

    xmlhttp.open("GET", page, true);
    xmlhttp.setRequestHeader("Content-type",
                             "application/x-www-form-urlencoded");
    xmlhttp.send();

    xmlhttp.onreadystatechange = function ()
    {
        if((xmlhttp.readyState == 4) && (xmlhttp.status == 200))
        {
            document.getElementById("content").innerHTML = xmlhttp.responseText;
        }
    }
}

//var HTTPrequest = new XMLHttpRequest();
function led_on()
{
	var params = "__SL_P_U00";
	params = params +"LED_ON";
	HTTPrequest.open("POST","No_content", true);
	HTTPrequest.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	HTTPrequest.setRequestHeader("Content-length", params.length);
	HTTPrequest.setRequestHeader("Connection", "close");
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
	HTTPrequest.setRequestHeader("Content-length", params.length);
	HTTPrequest.setRequestHeader("Connection", "close");
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
	HTTPrequest.setRequestHeader("Content-length", params.length);
	HTTPrequest.setRequestHeader("Connection", "close");
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
	HTTPrequest.setRequestHeader("Content-length", params.length);
	HTTPrequest.setRequestHeader("Connection", "close");
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
			HTTPrequest.setRequestHeader("Content-length", params.length);
			HTTPrequest.setRequestHeader("Connection", "close");
			HTTPrequest.onreadystatechange = function()
			{
				if (HTTPrequest.readyState==4 && HTTPrequest.status==200)
				{
				}
			}
			HTTPrequest.send(params);
            get_temperature();
		}
		function temperatureF()
		{
			var params = "__SL_P_U09";
			params = params + "TEMP_IN_F";
			HTTPrequest.open("POST","No_content", true);
			HTTPrequest.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
			HTTPrequest.setRequestHeader("Content-length", params.length);
			HTTPrequest.setRequestHeader("Connection", "close");
			HTTPrequest.onreadystatechange = function()
			{
				if (HTTPrequest.readyState==4 && HTTPrequest.status==200)
				{
				}
			}
			HTTPrequest.send(params);
            get_temperature();
 		}
		
		function speedSet()
		{
			var params = "__SL_P_U04";
            var speed_txt = document.getElementById("speed_percent").value;
			params = params+ "_" + speed_txt + "END";
			HTTPrequest.open("POST","No_content", true);
			HTTPrequest.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
			HTTPrequest.setRequestHeader("Content-length", params.length);
			HTTPrequest.setRequestHeader("Connection", "close");
			HTTPrequest.onreadystatechange = function()
			{
				if (HTTPrequest.readyState==4 && HTTPrequest.status==200)
				{
				}
			}
			HTTPrequest.send(params);
		}
		
//		function get_button1()
//		{
//			var params = "__SL_G_U05";
//			params = params+ "GET BUTTON 1 COUNT";
//            HTTPrequest.open("GET","No_content", true);
//			HTTPrequest.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
//			HTTPrequest.setRequestHeader("Content-length", params.length);
//			HTTPrequest.setRequestHeader("Connection", "close");
//			HTTPrequest.onreadystatechange = function()
//			{
//				if (HTTPrequest.readyState==4 && HTTPrequest.status==200)
//				{
//				}
//			}
//			HTTPrequest.send(params);
//			location.reload(true); 
//		}
		
//		function get_button2()
//		{
//			var params = "__SL_G_U06";
//           	HTTPrequest.open("GET","No_content", true);
//			HTTPrequest.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
//			HTTPrequest.setRequestHeader("Content-length", params.length);
//			HTTPrequest.setRequestHeader("Connection", "close");
//			HTTPrequest.onreadystatechange = function()
//			{
//				if (HTTPrequest.readyState==4 && HTTPrequest.status==200)
//				{
//				}
//			}
//			HTTPrequest.send(params);
//			document.getElementById("get_button2_count").innerHTML = __SL_G_U06;
//			location.reload(true); 
//		}
		
		function clear_button1()
		{
			var params = "__SL_P_U05";
			params = params+ "CLEAR BUTTON 1 COUNT";
            HTTPrequest.open("POST","No_content", true);
			HTTPrequest.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
			HTTPrequest.setRequestHeader("Content-length", params.length);
			HTTPrequest.setRequestHeader("Connection", "close");
			HTTPrequest.onreadystatechange = function()
			{
				if (HTTPrequest.readyState==4 && HTTPrequest.status==200)
				{
				}
			}
			HTTPrequest.send(params);
			document.getElementById("button1_count").value = "0";
		}
		
		function clear_button2()
		{
			var params = "__SL_P_U06";
            params = params+ "CLEAR BUTTON 2 COUNT";
           	HTTPrequest.open("POST","No_content", true);
			HTTPrequest.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
			HTTPrequest.setRequestHeader("Content-length", params.length);
			HTTPrequest.setRequestHeader("Connection", "close");
			HTTPrequest.onreadystatechange = function()
			{
				if (HTTPrequest.readyState==4 && HTTPrequest.status==200)
				{
				}
			}
			HTTPrequest.send(params);
			document.getElementById("button2_count").value = "0";
		}
		
//		function get_temperature()
//		{
//			var params = "__SL_G_U07";
//			params = params + "GET TEMPERATURE";
//            HTTPrequest.open("GET","No_content", true);
//			HTTPrequest.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
//			HTTPrequest.setRequestHeader("Content-length", params.length);
//			HTTPrequest.setRequestHeader("Connection", "close");
//			HTTPrequest.onreadystatechange = function()
//			{
//				if (HTTPrequest.readyState==4 && HTTPrequest.status==200)
//				{
//				}
//			}
//			HTTPrequest.send(params);  
//			document.getElementById("temperature").value = "__SL_GU007";
//
//		}
		
		
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

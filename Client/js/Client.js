document.addEventListener("DOMContentLoaded", init, false);

var connection;

function init()
{
    //var login = doument.getElementById("login");
    document.addEventListener("mousemove", getPosition, false);

    connection = new WebSocket('ws://127.0.0.1:8569/myapp');

    console.log("Dupa");
    console.log("Dupa", connection);

    connection.onopen = function(){
        //Send a small message to the console once the connection is established
        console.log('Connection open!');
    }

    connection.onclose = function(){
        console.log('Connection closed');
    }

    connection.onerror = function(error){
        console.log('Error detected: ' + error);
    }

    connection.onmessage = function(e){
        var server_message = e.data;
        console.log("Serwer: " + server_message);
    }
}

function getPosition(event)
{
    var x = new Number();
    var y = new Number();
    var table = document.getElementById("pos_table");
    var td = table.getElementsByTagName("td");

    if (event.x != undefined && event.y != undefined)
    {
        x = event.x;
        y = event.y;
    }
    else // Firefox method to get the position
    {
        x = event.clientX;
        y = event.clientY;
    }

    td[0].innerHTML = x;
    td[1].innerHTML = y;
}

function showTime(){
    document.getElementById("time").innerHTML = Date();
}

function login(){

    console.log("Loduje function");

    var login = document.getElementById("username");
    var psw = document.getElementById("pswd");
    var text = "Loguje jako: " + login.value + " z haslem " + psw.value;



    connection.send(text);

    //alert(text);

}
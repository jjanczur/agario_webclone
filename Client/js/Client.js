document.addEventListener("DOMContentLoaded", init, false);

function init()
{
    //var login = doument.getElementById("login");
    document.addEventListener("mousemove", getPosition, false);
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

    var login = document.getElementById("username");
    var psw = document.getElementById("pswd");
    var text = "Loguje jako: " + login.value + " z haslem " + psw.value;

    alert(text);

}

var CurrentLocation = "None";
var CurrentLocationDescription = "";
var CurrentLocationDecisions;

function SendDataRequest(index)
{
	var request = new XMLHttpRequest();
	request.open("RPG", "", true);
	request.onload = AsyncRequestOnload;
	request.send("location=" + CurrentLocation + "&decision=" + index);
}

function AsyncRequestOnload()
{
	var response = this.responseText.split("%%");
	CurrentLocation = response[0].split("=")[1];
	CurrentLocationDescription = response[1].split("=")[1];
	WriteLocationDescription();
	
	CurrentLocationDecisions = new Array();
	for (i = 0; i < response[2].split("=")[1]; i++)
	{
		CurrentLocationDecisions.push((i + 1) + ". " + response[i + 3]);
	}
	CreateGameButtons();
}

function WriteLocationDescription()
{
	var output = "";
	output += "<div id='mainTextDiv'>";
	var data = CurrentLocationDescription;
	var paragraphs = data.split("<br>");
	for (i = 0; i < paragraphs.length; i++)
	{
		output += "<p class='mainTextP'>" + paragraphs[i] + "</p>";
	}
	output += "</div>";
	document.getElementById('areaDescriptionDiv').innerHTML = output;
}

function CreateGameButtons()
{
	var output = "";
	for (i = 0; i < 6; i++)
	{
		var buttonData = CurrentLocationDecisions[i];
		if (buttonData != null)
		{
			output += "<button class='gameAnswerButton' onclick='OnButtonClick(" + i + ")'>" + buttonData + "</button>";
		}
	}
	document.getElementById('gameButtonsDiv').innerHTML = output;
}

function OnButtonClick(index)
{
	SendDataRequest(index);
}

// Prevent context menu from appearing
function OnContextMenu(event)
{
	event.preventDefault();
}
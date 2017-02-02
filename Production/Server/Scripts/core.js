
var sessionKey = undefined;
var currentLocation;
var currentLocationDescription = "";
var currentLocationDecisions;
var currentPlayerName;

var characterRace;
var characterGender;

var worldTime;

// Page init
function Init()
{
	document.getElementById('infoDiv').style.visibility = 'hidden';
}

// New game
function StartGame(action)
{
	currentPlayerName = document.getElementById('playerNameInput').value;
	// Character race
	var characterRaceSelect = document.getElementById('characterRace');
	characterRace = characterRaceSelect.options[characterRaceSelect.selectedIndex].value;
	// Character gender
	var genderRadio = document.getElementsByName('characterGender');
	if (genderRadio[0].checked)
		characterGender = "male";
	else
		characterGender = "female";

	// Start
	if (action == "New")
	{
		var select = document.getElementById("entryPointSelect");
		GameStartRequest(currentPlayerName, select.options[select.selectedIndex].value, characterRace, characterGender);
	}
	else if (action == "Resume")
	{
		GameResumeRequest(currentPlayerName);
	}
	document.getElementById('loginDiv').innerHTML = null;
	document.getElementById('infoDiv').style.visibility = 'visible';
}

// Game button press event handler
function OnButtonClick(index)
{
	MovementUpdateRequest(index);
}

// Prevent context menu from appearing
function OnContextMenu(event)
{
	event.preventDefault();
}
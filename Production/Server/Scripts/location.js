
function ApplyLocationData(locationGuid, locationDescription, decisionCount, decisionTexts)
{
	currentLocation = locationGuid;
	currentLocationDescription = locationDescription;
	WriteLocationDescription();
	
	currentLocationDecisions = new Array();
	for (i = 0; i < decisionCount; i++)
	{
		currentLocationDecisions.push((i + 1) + ". " + decisionTexts[i]);
	}
	CreateGameButtons();
	UpdateDebugInfo();
}

// Page loading - area description
function WriteLocationDescription()
{
	var output = "";
	output += "<div id='mainTextDiv' class='infoBox'>";
	var data = currentLocationDescription;
	var paragraphs = data.split("<p>");
	for (i = 1; i < paragraphs.length; i++)
	{
		// Check for empty paragraphs
		if (paragraphs[i] != "</p>")
		{
			// Remove line break in the beginning
			var paragraph = paragraphs[i];
			
			// Add paragraph data
			output += "<p class='mainTextP'>" + paragraph;
			// </p> is included in the paragraph itself
		}
	}
	output += "</div>";
	document.getElementById('areaDescriptionDiv').innerHTML = output;
}

// Page loading - button creation
function CreateGameButtons()
{
	var output = "";
	for (i = 0; i < 10; i++)
	{
		var buttonData = currentLocationDecisions[i];
		if (buttonData != null && buttonData != "")
		{
			output += "<button class='gameAnswerButton' onclick='OnButtonClick(" + i + ")'>" + buttonData + "</button>";
		}
	}
	document.getElementById('gameButtonsDiv').innerHTML = output;
}

// Debug information
function UpdateDebugInfo()
{
	document.getElementById('debugPlayerNameP').innerHTML = "Имя: " + currentPlayerName;
	//document.getElementById('debugSessionIndexP').innerHTML = "Ключ сессии: " + sessionKey;
	//document.getElementById('debugLocationNameP').innerHTML = "Локация: " + currentLocation;

	var race = characterRace;
	if (characterRace == 'tairan')
		race = "Тайран";
	else if (characterRace == 'helvera')
		race = "Хельвера";
	document.getElementById('debugCharacterRaceP').innerHTML = "Раса: " + race;

	var gender;
	if (characterGender == 'male')
		gender = "Мужской";
	else
		gender = "Женский";
	document.getElementById('debugCharacterGenderP').innerHTML = "Пол: " + gender;

	var time = worldTime;
	if (worldTime == 'day')
		time = "День";
	else if (worldTime == 'night')
		time = "Ночь";
	document.getElementById('debugWorldTimeP').innerHTML = "Время суток: " + time;
}
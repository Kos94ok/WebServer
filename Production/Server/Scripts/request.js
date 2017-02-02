
// Request new game session initialization
function GameStartRequest(name, entryPoint, race, gender)
{
	var request = new XMLHttpRequest();
	request.open("RPG", "game.html", true);
	request.onload = InitialData_Onload;
	request.send("req=entry&point=" + entryPoint + "&player=" + name + "&race=" + race + "&gender=" + gender);
}

// Request previous game session resume
function GameResumeRequest(playerName)
{
	var request = new XMLHttpRequest();
	request.open("RPG", "game.html", true);
	request.onload = InitialData_Onload;
	request.send("req=resume&player=" + playerName);
}

function InitialData_Onload()
{
	sessionKey = GetKeyValue(this.responseText, "session");
	LocationDataRequest();
	CharacterDataRequest();
	WorldDataRequest();
}

// Request location data from the server
function LocationDataRequest()
{
	var request = new XMLHttpRequest();
	request.open("RPG", "game.html", true);
	request.onload = LocationData_Onload;
	request.send("req=locationData&session=" + sessionKey);
}

// Request movement update for the pressed button
function MovementUpdateRequest(pressedButton)
{
	var request = new XMLHttpRequest();
	request.open("RPG", "game.html", true);
	request.onload = LocationData_Onload;
	request.send("req=decision&session=" + sessionKey + "&location=" + currentLocation + "&decision=" + pressedButton);
}

function LocationData_Onload()
{
	var characterUpdated = GetKeyValue(this.responseText, "isCharacterUpdated");
	var locationGuid = GetKeyValue(this.responseText, "guid");
	var locationDescription = GetKeyValue(this.responseText, "description");
	var decisionCount = GetKeyValue(this.responseText, "decCount");
	var fieldCount = 3;
	if (characterUpdated != null)
		fieldCount += 1;
	var decisionTexts = this.responseText.split("%%").slice(fieldCount);

	ApplyLocationData(locationGuid, locationDescription, decisionCount, decisionTexts);
}

// Ask for character details
function CharacterDataRequest()
{
	var request = new XMLHttpRequest();
	request.open("RPG", "game.html", true);
	request.onload = CharacterDataRequest_Onload;
	request.send("req=characterData&session=" + sessionKey);
}

function CharacterDataRequest_Onload()
{
	characterGender = GetKeyValue(this.responseText, "gender");
	characterRace = GetKeyValue(this.responseText, "race");
	UpdateDebugInfo();
}

// Ask for current world status
function WorldDataRequest()
{
	var request = new XMLHttpRequest();
	request.open("RPG", "game.html", true);
	request.onload = WorldDataRequest_Onload;
	request.send("req=worldData&session=" + sessionKey);
}

function WorldDataRequest_Onload()
{
	worldTime = GetKeyValue(this.responseText, "time");
	UpdateDebugInfo();
}
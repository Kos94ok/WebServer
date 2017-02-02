
function GetKeyValue(ResponseText, Key)
{
	var Response = ResponseText.split("%%");
	var Delimiter = "=";

	for (var i = 0; i < Response.length; i++)
	{
		if (Response[i].indexOf(Delimiter) != -1)
		{
			var Left = Response[i].substring(0, Response[i].indexOf(Delimiter));
			var Right = Response[i].substring(Response[i].indexOf(Delimiter) + 1);
			if (Left == Key)
				return Right;
		}
	}
	return null;
}

function RaceInfoOnMouseEnter()
{
	var Window = document.getElementById('raceInfoWindow');
	var Question = document.getElementById('raceInfoQuestion');
	Window.style.left = Question.getBoundingClientRect().left + 'px';
	Window.style.top = Question.getBoundingClientRect().top + 'px';
	Window.style.visibility = 'visible';
	Window.style.opacity = 1;
	var characterRaceSelect = document.getElementById('characterRace');
	characterRace = characterRaceSelect.options[characterRaceSelect.selectedIndex].value;

	Window.innerHTML = "";
	if (characterRace == 'tairan')
	{
		Window.innerHTML += "<p class='raceInfoWindowHeader'>Тайран</p>";
		Window.innerHTML += "<p class='raceInfoWindowText'>Невысокие люди, как правило, приветливые и добродушные, особенно по отношению к родственникам. Волосы светлых оттенков, вплоть до полностью белого, цвет кожи достаточно светлый.</p>";
		Window.innerHTML += "<p class='raceInfoWindowText'>Большинство тайранов носит длинные волосы. Часто заплетают косы, хвосты или иные виды причесок, вплоть до очень замысловатых. Ни в одежде, ни в аксессуарах, ни в прическах нет разделения на мужские и женские.</p>";
		Window.innerHTML += "<p class='raceInfoWindowText'>Часто носят плотные кожаные одежды, в том числе плащи, перчатки, сапоги и брюки. Как правило, плащ имеет капюшон для особо солнечных или дождливых дней. Под плащом может быть надета рубашка или нечто иное, из более удобной ткани, чем кожа. Это может быть как хлопок, так и шелк. Часто носят амулеты или кулоны с семейным гербом, и считают принадлежность к своей семье поводом для гордости.</p>";
		Window.innerHTML += "<p class='raceInfoWindowText'>Будучи крайне привязанными к своим семьям, стараются работать на благо родственников, чтобы оставить достойное наследство будущим поколениям. Иметь в семье до десяти детей считается совершенно нормальным, и родители с удовольствием учат отпрысков своему ремеслу, стараясь передать как можно больше знаний. Дети же практически всегда образуют ремесленные династии и принимают профессии своих родителей.</p>";
	}
	else if (characterRace == 'helvera')
	{
		Window.innerHTML += "<p class='raceInfoWindowHeader'>Хельвера</p>";
		Window.innerHTML += "<p class='raceInfoWindowText'>Невысокие и стройные люди, ведущие ночной образ жизни. Кожа светлая, с голубоватым оттенком, доминирующий цвет волос – от белого до выраженного синего, с различными оттенками.</p>";
		Window.innerHTML += "<p class='raceInfoWindowText'>Женщины носят длинные волосы, вплетая в них цветы или драгоценные камни, и заплетая причудливые косы или другие прически. Мужские волосы, как правило, обстрижены до уровня плеч, но могут быть как длинными, так и совсем короткими.</p>";
		Window.innerHTML += "<p class='raceInfoWindowText'>Одежду носят длинную, девушки часто предпочитают платья с прямым силуэтом, как правило, с украшениями в виде поясов, ожерелий или подвесок. Мужская одежда отличается только длиной. Если женское платье может быть длиной до пола, или даже длиннее, то мужская будет, как правило, до колена или чуть выше.</p>";
		Window.innerHTML += "<p class='raceInfoWindowText'>Хельверские города располагаются в местах, добраться до которых может быть крайне сложно. Горные пики, каньоны и прочие уединенные места являются идеальным расположением для их небольших городов. Сами хельвера не чувствуют особой привязанности к родным местам, и достаточно легко отделяются от своих семей.</p>";
	}
	else
	{
		Window.innerHTML = "missingno";
	}
}

function RaceInfoOnMouseLeave()
{
	var Window = document.getElementById('raceInfoWindow');
	Window.style.opacity = 0;
	var timerId = setTimeout(RaceInfoOnHide, 100);
}

function RaceInfoOnHide()
{
	var Window = document.getElementById('raceInfoWindow');
	Window.innerHTML = "";
}
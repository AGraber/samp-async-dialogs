# samp-async-dialogs

[![sampctl](https://shields.southcla.ws/badge/sampctl-samp--async--dialogs-2f2f2f.svg?style=for-the-badge)](https://github.com/AGraber/samp-async-dialogs)

Async dialog handling with PawnPlus tasks.

## Installation

Simply install to your project:

```bash
sampctl package install AGraber/samp-async-dialogs
```

Include in your code and begin using the library:

```pawn
#include <async-dialogs>
```

It is recommended that you set a PawnPlus version explicitely on your pawn.json
(preferibly the latest) to avoid always downloading the latest one.

If you don't use sampctl, just download the `async-dialogs.inc` include and
drop it to your `includes/` folder, and then download the PawnPlus plugin and
include [from here](https://github.com/IllidanS4/PawnPlus/releases).

While you're on it and if you don't use PawnPlus yet,
[you should check it out!](https://github.com/IllidanS4/PawnPlus/blob/master/README.md)

## Usage

This include provides one single function
```pawn
ShowPlayerAsyncDialog(playerid, style, const caption[], const info[], const button1[], const button2[])
```

This will show the dialog and await for the response, which will pause the
current script's execution and return the yielded value to the last public
function (or 0 if it wasn't set). When it's responded to, the response details
will be inside the `dialog_response[e_DIALOG_RESPONSE_INFO]` array. If another
dialog gets shown while awaiting, the Task will be discarded with any following
code that was to be resumed.

### Example command:
```pawn
CMD:asyncdialog(playerid, params[])
{
	task_yield(1);

	new dialog_response[e_DIALOG_RESPONSE_INFO];
	await_arr(dialog_response) ShowPlayerAsyncDialog(playerid, DIALOG_STYLE_LIST, "Example dialog", "This is listitem 0\nAnd this is one\nShow example nested dialog", "ok", "no");

	if(dialog_response[E_DIALOG_RESPONSE_Response])
	{
		if(dialog_response[E_DIALOG_RESPONSE_Listitem] == 2)
		{
			new other_dialog_response[e_DIALOG_RESPONSE_INFO];
			await_arr(other_dialog_response) ShowPlayerAsyncDialog(playerid, DIALOG_STYLE_MSGBOX, "Example nested dialog", "You will recieve a message\nwhichever your response is", "OK", "Yes");

			SendClientMessage(playerid, COLOR_WHITE, "You responded something to the example dialog");
		}
		else
		{
			SendClientMessageEx(playerid, COLOR_WHITE, "Recieved: listitem = %d | inputtext = %s", dialog_response[E_DIALOG_RESPONSE_Listitem], dialog_response[E_DIALOG_RESPONSE_InputText]);
		}
	}
	else
	{
		SendClientMessage(playerid, COLOR_WHITE, "Bye!");
	}
}
```

## Thanks
* @IllidanS4 for PawnPlus, which allows this and even more awesome stuff
* @TommyB123 for helping me test it out on a real environment


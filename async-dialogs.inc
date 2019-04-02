#if defined _inc_async_dialogs
	#undef _inc_async_dialogs
#endif 

#if defined async_dialogs_included
	#endinput
#endif
#define async_dialogs_included

#include <PawnPlus>

enum e_DIALOG_RESPONSE_INFO
{
	E_DIALOG_RESPONSE_Response,
	E_DIALOG_RESPONSE_Listitem,
	E_DIALOG_RESPONSE_InputText[128 + 1],
}

const PAWN_PLUS_DIALOG_ID = 0x502B; // "P+" in ascii hex

static Task:PlayerDialogTasks[MAX_PLAYERS] = {Task:0, ...};

public OnGameModeInit()
{
	pawn_register_callback("OnDialogResponse", "AsyncDialogsHandler");
	pawn_add_filter("ShowPlayerDialog", "dddssss", "AsyncDialogsShowHook", filter_out);

	#if defined AsyncDialogs_OnGameModeInit
		return AsyncDialogs_OnGameModeInit();
	#else
		return 1;
	#endif
}

AwaitAsyncDialog(playerid, dialog_response[e_DIALOG_RESPONSE_INFO], style, const caption[], const info[], const button1[], const button2[])
{
	ShowPlayerDialog(playerid, PAWN_PLUS_DIALOG_ID, style, caption, info, button1, button2);
	new const Task:dialog_task = PlayerDialogTasks[playerid] = task_new();
	task_await(dialog_task);

	task_get_result_arr(dialog_task, dialog_response);
}

forward AsyncDialogsHandler(playerid, dialogid, response, listitem, const inputtext[]);
public AsyncDialogsHandler(playerid, dialogid, response, listitem, const inputtext[])
{
	if(dialogid == PAWN_PLUS_DIALOG_ID)
	{
		new dialog_response[e_DIALOG_RESPONSE_INFO];
		dialog_response[E_DIALOG_RESPONSE_Response] = response;
		dialog_response[E_DIALOG_RESPONSE_Listitem] = listitem;
		strcat((dialog_response[E_DIALOG_RESPONSE_InputText][0] = '\0', dialog_response[E_DIALOG_RESPONSE_InputText]), inputtext);

		new const Task:task = PlayerDialogTasks[playerid];
		PlayerDialogTasks[playerid] = Task:0;
		task_set_result_arr(task, dialog_response);

		return 1;
	}
	return 0;
}

forward AsyncDialogsShowHook(&playerid, &dialogid, &style, const caption[], const info[], const button1[], const button2[]);
public AsyncDialogsShowHook(&playerid, &dialogid, &style, const caption[], const info[], const button1[], const button2[])
{
	if(PlayerDialogTasks[playerid])
	{
		task_delete(PlayerDialogTasks[playerid]);
		PlayerDialogTasks[playerid] = Task:0;
	}
}

#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit AsyncDialogs_OnGameModeInit
#if defined AsyncDialogs_OnGameModeInit
    forward AsyncDialogs_OnGameModeInit();
#endif

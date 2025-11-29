#include "precompiled.h"

CMatchCommand gMatchCommand;

// On Server Activate
void CMatchCommand::ServerActivate()
{
	// Clear
	this->m_Data.clear();
	
	// Memory Script instance
	CMemScript* lpMemScript = new CMemScript;

	// If is not null
	if (lpMemScript)
	{
		// Try to load file
		if (lpMemScript->SetBuffer(MB_COMMANDS_FILE))
		{
			try
			{
				// Loop lines
				while (true)
				{
					// If file content ended
					if (lpMemScript->GetToken() == eTokenResult::TOKEN_END)
					{
						// Break loop
						break;
					}

					P_COMMAND_INFO Info = { };

					// Command Index
					Info.Index = lpMemScript->GetNumber();

					//  Command String
					Info.Name = lpMemScript->GetAsString();

					// Command Flag
					Info.Flag = gMatchAdmin.ReadFlags(lpMemScript->GetAsString().c_str());

					// Insert to container
					this->m_Data.insert(std::make_pair(Info.Name , Info));
				}
			}
			catch (...)
			{
				// Catch for erros
				gpMetaUtilFuncs->pfnLogConsole(PLID, "[%s][%s] %s", __func__, MB_COMMANDS_FILE, lpMemScript->GetError().c_str());
			}
		}

		// Delete Memory Script instance
		delete lpMemScript;
	}
}

// Get Command info
LP_COMMAND_INFO CMatchCommand::GetInfo(E_COMMAND_INDEX CommandIndex)
{
	// Loop registered commands
	for (auto const& Command : this->m_Data)
	{
		// If index is the same
		if (CommandIndex == Command.second.Index)
		{
			// Return pointer to command data
			return &this->m_Data[Command.first];
		}
	}

	// Return null pointer
	return nullptr;
}

// On Client command
bool CMatchCommand::ClientCommand(CBasePlayer* Player, const char* pcmd, const char* parg1)
{
	// IF is not null
	if (!pcmd)
	{
		return false;
	}

	// If string is not empty
	if (pcmd[0u] == '\0')
	{
		return false;
	}

	// If is menuselect command
	if (Q_stricmp(pcmd,"menuselect") == 0)
	{
		// If has arguments
		if (parg1)
		{
			// If native CS menu is not being displayed
			if (Player->m_iMenu == Menu_OFF)
			{
				// Handle menu
				if (gMatchMenu[Player->entindex()].Handle(Player->entindex(), Q_atoi(parg1)))
				{
					// Return result
					return true;
				}
			}
		}
	}
	// If player used say or say_team command
	else if (Q_stricmp(pcmd, "say") == 0 || Q_stricmp(pcmd, "say_team") == 0)
	{
		if (parg1)
		{
			// Check if message starts with '/'
			if (parg1[0u] == '/')
			{
				// Get the full command line
				auto pCmdArgs = g_engfuncs.pfnCmd_Args();
				
				if (!pCmdArgs || pCmdArgs[0u] == '\0')
				{
					return true;
				}
				
				// Skip the '/' and get the command name
				const char* pCommand = parg1 + 1;
				
				// Find space to separate command from arguments
				std::string fullCommand = pCmdArgs;
				std::string commandName = pCommand;
				std::string arguments = "";
				
				size_t spacePos = commandName.find(' ');
				if (spacePos != std::string::npos)
				{
					arguments = commandName.substr(spacePos + 1);
					commandName = commandName.substr(0, spacePos);
				}
				
				// Look up the command
				auto const Command = this->m_Data.find(commandName.c_str());
				
				if (Command != this->m_Data.end())
				{
					// Check permissions
					if (Command->second.Flag)
					{
						auto Flags = gMatchAdmin.GetFlags(Player->entindex());
						
						if (!(Flags & Command->second.Flag))
						{
							gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("You do not have access to that command."));
							return true;
						}
					}
					
					// For commands that need arguments, temporarily set them
					// by creating a fake command line
					if (Command->second.Index == CMD_ADMIN_MESSAGE || Command->second.Index == CMD_ADMIN_COMMAND)
					{
						// Inject the arguments into a temp buffer that Message/Rcon can read
						if (!arguments.empty())
						{
							// Execute via ClientCommand with proper arguments
							char tempCmd[256];
							Q_snprintf(tempCmd, sizeof(tempCmd), "%s %s", commandName.c_str(), arguments.c_str());
							
							// Store in a way Message/Rcon can retrieve
							// Use ServerCommand to set a temporary variable or pass directly
						}
					}
					
					// Execute the command switch
					switch (Command->second.Index)
					{
						case CMD_PLAYER_STATUS:
						{
							gMatchBot.Status(Player);
							return true;
						}
						case CMD_PLAYER_SCORE:
						{
							gMatchBot.Scores(Player, false);
							return true;
						}
						case CMD_PLAYER_READY:
						{
							gMatchReady.Ready(Player);
							return true;
						}
						case CMD_PLAYER_NOTREADY:
						{
							gMatchReady.NotReady(Player);
							return true;
						}
						case CMD_PLAYER_HP:
						{
							gMatchRound.ShowHP(Player, true, false);
							return true;
						}
						case CMD_PLAYER_DMG:
						{
							gMatchRound.ShowDamage(Player, true, false);
							return true;
						}
						case CMD_PLAYER_RDMG:
						{
							gMatchRound.ShowReceivedDamage(Player, true, false);
							return true;
						}
						case CMD_PLAYER_SUM:
						{
							gMatchRound.ShowSummary(Player, true, false);
							return true;
						}
						case CMD_PLAYER_HELP:
						{
							gMatchBot.Help(Player, false);
							return true;
						}
						case CMD_PLAYER_VOTE:
						{
							gMatchVoteMenu.Menu(Player);
							return true;
						}
						case CMD_PLAYER_VOTE_KICK:
						{
							gMatchVoteMenu.VoteKick(Player);
							return true;
						}
						case CMD_PLAYER_VOTE_MAP:
						{
							gMatchVoteMenu.VoteMap(Player);
							return true;
						}
						case CMD_PLAYER_VOTE_PAUSE:
						{
							gMatchVoteMenu.VotePause(Player);
							return true;
						}
						case CMD_PLAYER_VOTE_RESTART:
						{
							gMatchVoteMenu.VoteRestart(Player);
							return true;
						}
						case CMD_PLAYER_VOTE_STOP:
						{
							gMatchVoteMenu.VoteStop(Player);
							return true;
						}
						case CMD_PLAYER_MUTE_MENU:
						{
							gMatchMute.Menu(Player);
							return true;
						}
						case CMD_PLAYER_VOTE_SURRENDER:
						{
							gMatchVoteMenu.VoteSurrender(Player);
							return true;
						}
						case CMD_ADMIN_MENU:
						{
							gMatchAdminMenu.MainMenu(Player->entindex());
							return true;
						}
						case CMD_ADMIN_KICK:
						{
							gMatchAdminMenu.KickMenu(Player->entindex());
							return true;
						}
						case CMD_ADMIN_BAN:
						{
							gMatchAdminMenu.BanMenu(Player->entindex());
							return true;
						}
						case CMD_ADMIN_KILL:
						{
							gMatchAdminMenu.SlayMenu(Player->entindex());
							return true;
						}
						case CMD_ADMIN_TEAM:
						{
							gMatchAdminMenu.TeamMenu(Player->entindex());
							return true;
						}
						case CMD_ADMIN_MAP:
						{
							gMatchAdminMenu.MapMenu(Player->entindex());
							return true;
						}
						case CMD_ADMIN_CONTROL:
						{
							gMatchAdminMenu.ControlMenu(Player->entindex());
							return true;
						}
						case CMD_ADMIN_MESSAGE:
						{
							if (!arguments.empty())
							{
								gMatchAdminMenu.Message(Player, arguments.c_str());
							}
							else
							{
								gMatchAdminMenu.Message(Player, nullptr);
							}
							return true;
						}
						case CMD_ADMIN_COMMAND:
						{
							if (!arguments.empty())
							{
								gMatchAdminMenu.Rcon(Player, arguments.c_str());
							}
							else
							{
								gMatchAdminMenu.Rcon(Player, nullptr);
							}
							return true;
						}
						case CMD_ADMIN_SWAP:
						{
							gMatchAdminMenu.SwapTeams(Player->entindex());
							return true;
						}
						case CMD_ADMIN_VOTE_MAP:
						{
							gMatchBot.StartVoteMap(Player);
							return true;
						}
						case CMD_ADMIN_VOTE_TEAM:
						{
							gMatchBot.StartVoteTeam(Player);
							return true;
						}
						case CMD_ADMIN_START_MATCH:
						{
							gMatchBot.StartMatch(Player);
							return true;
						}
						case CMD_ADMIN_STOP_MATCH:
						{
							gMatchBot.StopMatch(Player);
							return true;
						}
						case CMD_ADMIN_RESTART_MATCH:
						{
							gMatchBot.RestartMatch(Player);
							return true;
						}
						case CMD_ADMIN_PAUSE_MATCH:
						{
							gMatchPause.Init(Player, UNASSIGNED);
							return true;
						}
						case CMD_ADMIN_HELP:
						{
							gMatchBot.Help(Player, true);
							return true;
						}
						case CMD_ADMIN_PLAYER_LIST:
						{
							gMatchPlayer.PlayerMenu(Player);
							return true;
						}
						case CMD_ADMIN_CVAR_MENU:
						{
							gMatchCvarMenu.Menu(Player);
							return true;
						}
					}
				}
				
				// Block the message from showing in chat
				return true;
			}
		}
	}
	// Remove the else block completely - no console commands

	// Allow in chat
	return false;
}

class npc_mmr_reset : public CreatureScript
{
    public:
        npc_mmr_reset() : CreatureScript("npc_mmr_reset") { }

        uint16* GetMmr(Player *player)
        {
            uint16 *mmr = new uint16[3];
            mmr[0] = 0;
            mmr[1] = 0;
            mmr[2] = 0;
            for(int x = 0; x < 3; x++)
            {
                if (ArenaTeam *twos = sArenaTeamMgr->GetArenaTeamById(player->GetArenaTeamId(x)))
                    mmr[x] = twos->GetMember(player->GetGUID())->MatchMakerRating;
                else
                    mmr[x] = 0;
            }
            return mmr;
        }
        /*
        // change personal rating
        bool ChangePersonalRating(Player *player, int value, int slot)
        {
            if (ArenaTeam *team = sArenaTeamMgr->GetArenaTeamById(player->GetArenaTeamId(slot)))
            {
                //int32 oldPersonalRating = 0;
                ArenaTeamMember *member = team->GetMember(player->GetGUID());
                member->PersonalRating = value;
                member->ModifyPersonalRating(player, value - (int)member->PersonalRating, slot);
                team->SaveToDB();
                return true;
            }
            return false;
        }
        */

        bool ChangeMmr(Player *player, int slot, int value)
        {
            if (ArenaTeam *team = sArenaTeamMgr->GetArenaTeamById(player->GetArenaTeamId(slot)))
            {
                ArenaTeamMember *member = team->GetMember(player->GetGUID());
                member->MatchMakerRating = value;
                //member->PersonalRating = value;
                //member->ModifyPersonalRating(player, value - (int)member->PersonalRating, slot);
                member->ModifyMatchmakerRating(value - (int)member->MatchMakerRating, slot);
                /*
                if (CaptainGuid == member)
                {
                    team->Disband(WorldSession* session);
                }
                else
                {
                    team->DelMember(player->GetGUID(), true);
                }
                */
                team->DelMember(player->GetGUID(), true);
                team->SaveToDB();
                return true;
            }
            return false;
        }

        bool OnGossipHello(Player *player, Creature *_creature)
        {
            uint16 *mmr = GetMmr(player);

            if (mmr[0] > 0)
            {
                if (mmr[0] >= 1950)
                    player->ADD_GOSSIP_ITEM(0, "Lower 2v2 MMR to 1950 and leave team!(CAN'T be team captain)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                else
                    player->ADD_GOSSIP_ITEM(0, "Reset 2v2 MMR to 1500 and leave team!(CAN'T be team captain)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            }

            if (mmr[1] > 0)
            {
                if (mmr[1] >= 1800)
                    player->ADD_GOSSIP_ITEM(0, "Lower 3v3 MMR to 1800 and leave team!(CAN'T be team captain)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
                else
                    player->ADD_GOSSIP_ITEM(0, "Reset 3v3 MMR to 1500 and leave team!(CAN'T be team captain)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            }

            if (mmr[2] > 0)
            {
                if (mmr[1] >= 1650)
                    player->ADD_GOSSIP_ITEM(0, "Lower 5v5 MMR to 1650 and leave team!(CAN'T be team captain)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
                else
                    player->ADD_GOSSIP_ITEM(0, "Reset 5v5 MMR to 1500 and leave team!(CAN'T be team captain)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
            }

            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Bye", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+7);
            player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _creature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player *player, Creature *_creature, uint32 sender, uint32 action)
        {
            if (action < GOSSIP_ACTION_INFO_DEF + 7)
            {
                uint16 *mmr = GetMmr(player);
                switch (action - GOSSIP_ACTION_INFO_DEF)
                {
                    case 1:  // Reset 2vs2 mmr to 1500
                        if (mmr[0] > 0)
                        {
                            if (ChangeMmr(player, 0, 1500))
                            {
                                //if(ChangePersonalRating(player, 1000, 0));
                                //{
                                player->SaveToDB();
                                //}
                            }
                        }
                        break;
                    case 2:  // Reset 2vs2 mmr to 1950
                        if (mmr[0] >= 1950)
                        {
                            if (ChangeMmr(player, 0, 1950))
                            {
                                //if(ChangePersonalRating(player, 1000, 0));
                                //{
                                player->SaveToDB();
                                //}
                            }
                        }
                        break;
                    case 3:  // Reset 3vs3 mmr to 1500
                        if (mmr[1] > 0)
                        {
                            if (ChangeMmr(player, 1, 1500))
                            {
                                //if(ChangePersonalRating(player, 1000, 1));
                                //{
                                player->SaveToDB();
                                //}
                            }
                        }
                        break;
                    case 4:  // Reset 3vs3 mmr to 1750
                        if (mmr[1] >= 1800)
                        {
                            if (ChangeMmr(player, 1, 1800))
                            {
                                //if(ChangePersonalRating(player, 1000, 1));
                                //{
                                player->SaveToDB();
                                //}
                            }
                        }
                        break;
                    case 5: // Reset 5vs5 mmr to 1500
                        if (mmr[2] > 0)
                        {
                            if (ChangeMmr(player, 2, 1500))
                            {
                                //if(ChangePersonalRating(player, 1000, 2));
                                //{
                                player->SaveToDB();
                                //}
                            }
                        }
                        break;
                    case 6:  // Reset 5vs5 mmr to 1650
                        if (mmr[2] >= 1650)
                        {
                            if (ChangeMmr(player, 2, 1650))
                            {
                                //if(ChangePersonalRating(player, 1000, 2));
                                //{
                                player->SaveToDB();
                                //}
                            }
                        }
                        break;
                    default:
                        break;
                }
            }

            player->CLOSE_GOSSIP_MENU();
            return true;
        }
};

void AddSC_npc_mmr_reset()
{
    new npc_mmr_reset;
}
Player::Player(int startX, int startY, StudentWorld* s)
: Actor(IID_PLAYER, startX, startY, s)
{
	set_status(true);
	set_Actor_vis(true);
	set_max_bug_sprayers(2);
	set_avail_bug_sprayers(max_bug_sprayers);
	set_walk_thru_bricks(false);
	set_extra_bug_sprayers(false);
	walk_thru_ticks = 0;
	extra_bug_sprayer_ticks = 0;
}

int Player::performAction()
{
	//.........

	cout << "max " << max_bug_sprayers << endl;
	cout << "avail " << available_bug_sprayers << endl;

	//........

	if (get_extra_bug_sprayers())
	{
		//if (get_avail_bug_sprayers() >= get_max_bug_sprayers())
		//set_avail_bug_sprayers(max_bug_sprayers);

		//decrement simultaneous bug sprayers tick by one
		extra_bug_sprayer_ticks--;

		if (extra_bug_sprayer_ticks <= 0)
		{
			set_max_bug_sprayers(2);
			set_avail_bug_sprayers(max_bug_sprayers);
			set_extra_bug_sprayers(false);
		}
	}

	//....//

	//gets the key pressed by user
	if (get_SWorld()->getKey(ch))
	{
		//........//
		case KEY_PRESS_SPACE:
			if (available_bug_sprayers > 0)
			{
				get_SWorld()->insert_Actor(new BugSprayer(getX(), getY(), get_SWorld()));
				available_bug_sprayers--;
			}
			break;
		}
	}
	return GWSTATUS_CONTINUE_GAME;
}

int BugSprayer::performAction()
{
	//cout << m_sprayer_lifetime << endl;
	//.......

			if (get_sprayer_lifetime() == 0)
			{
				//.......
				set_status(false);
				int max = get_SWorld()->get_Player()->get_max_bug_sprayers();
				int avail = get_SWorld()->get_Player()->get_avail_bug_sprayers();

				//after bugsprayer is blown up, the amount of bombs the player has
				//should increment by 1
				if (avail < max)
					get_SWorld()->get_Player()->inc_avail_bug_sprayers_by_one();
			}
		}
	}

	return GWSTATUS_CONTINUE_GAME;
}

///////////////IncSimulSprayerGoodie Implementation/////////////////////
IncSimulSprayerGoodie::IncSimulSprayerGoodie(int startX, int startY, StudentWorld* s)
: Actor(IID_INCREASE_SIMULTANEOUS_SPRAYER_GOODIE, startX, startY, s)
{
	set_status(true);
	set_Actor_vis(true);
	m_timer = get_SWorld()->get_inc_simul_lifetime();
}

int IncSimulSprayerGoodie::performAction()
{
	if (get_status())
	{
		if (m_timer > 0)
			m_timer--;
		else
		{
			m_timer = 0;
			set_status(false);
		}
		//check if player and goodie are in same square
		if (get_SWorld()->occupy_with_player(getX(), getY()))
		{
			//.......
			//get_SWorld() retrieves a pointer to my StudentWorld class, which is the game engine
			//that stores all the currently alive characters
			//using get_SWorld() to get a pointer to my Player (main char)
			get_SWorld()->get_Player()->set_extra_bug_sprayers(true);
			get_SWorld()->get_Player()->set_extra_bug_sprayers_ticks(ticks);
			get_SWorld()->get_Player()->set_max_bug_sprayers(sprayer_num);

			//checking to see how many of a particular character exists
			//using this to prevent adding more bombs to Player when continuously picking up goodies
			//prevents infinite bomb placing
			if (get_SWorld()->how_many_exist(IID_INCREASE_SIMULTANEOUS_SPRAYER_GOODIE) == 1)
				get_SWorld()->get_Player()->set_avail_bug_sprayers(sprayer_num);

			//.......
		}

	}
	else
		return GWSTATUS_CONTINUE_GAME;
}


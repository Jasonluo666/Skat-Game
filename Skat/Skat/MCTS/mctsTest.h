#pragma once
#include "ofxMSAmcts.h"
using namespace msa::mcts;

namespace test {
	// play card action
	struct Action {
		int suit;
		int value;
	};

	class State {
	public:

		//--------------------------------------------------------------
		// MUST HAVE METHODS (INTERFACE)

		State() {
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 8; j++) {
					data.cards[i][j] = false;
					//data.gameState[i][j] = false;
				}
			data.winValue[0] = data.winValue[1] = data.winValue[2] = 0;
		}

		State(const State& other) {
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 8; j++) {
					data.cards[i][j] = other.data.cards[i][j];
					//data.gameState[i][j] = other.data.gameState[i][j];
				}
			data.winValue[0] = other.data.winValue[0];
			data.winValue[1] = other.data.winValue[1];
			data.winValue[2] = other.data.winValue[2];
		}

		// default constructors will do
		// copy and assignment operators should perform a DEEP clone of the given state
		//    State(const State& other);
		//    State& operator = (const State& other);


		// whether or not this state is terminal (reached end)
		bool is_terminal() const {
			return false;
		}

		//  agent id (zero-based) for agent who is about to make a decision
		int agent_id() const {

			return 0;
		}

		// apply action to state
		void apply_action(const Action& action) {
			data.cards[action.suit][action.value] = false;
			//data.gameState[action.suit][action.value] = false;

			data.currentState[data.playSequence] = action.suit * 10 + action.value;
		}


		// return possible actions from this state
		void get_actions(std::vector<Action>& actions) const {
			int action_number = 0;
			int suit;

			switch (data.playSequence) {
			case 0:
				for(int suit = 0 ; suit < 4 ; suit++)
					for (int value = 0; value < 8; value++) {
						if (data.cards[suit][value] == true) {
							Action* action = new Action();
							action->suit = suit;
							action->value = value;

							actions[action_number++] = *action;
						}
					}
				break;
			case 1:
				suit = (data.currentState[0] % 10 == 0) ? data.trump : (data.currentState[0] / 10);


				break;
			case 2:
				suit = (data.currentState[0] % 10 == 0) ? data.trump : (data.currentState[0] / 10);


				break;
			}
		}


		// get a random action, return false if no actions found
		bool get_random_action(Action& action) const {

			return true;
		}


		// evaluate this state and return a vector of rewards (for each agent)
		const vector<float> evaluate() const {
			vector<float> rewards(3);

			if (data.winValue[data.declarer_id] > 61)
				for (int i = 0; i < 3; i++) {
					if (i == data.declarer_id)
						rewards[i] = 1;
					else
						rewards[i] = 0;
				}
			else
				for (int i = 0; i < 3; i++) {
					if (i == data.declarer_id)
						rewards[i] = 0;
					else
						rewards[i] = 1;
				}

			return rewards;
		}

	private:
		struct {
			bool cards[4][8];
			//gameState => cards
			int winValue[3];
			int agent_id;
			int declarer_id;
			int currentState[3];
			int playSequence;
			int trump;
		} data;
	};
}
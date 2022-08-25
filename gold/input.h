#pragma once

using BYTE = unsigned char;

/*
* Input manager
*/
class gold_input
{
  private:
	bool key_states[255] {};
	bool key_prev_states[255] {};

  public:
	/*
	* Handle key states this frame, call once every frame
	*/
	void run();

	/*
	 * <param name="key_code">win32 key code</param>
	 * <returns>Whether key code has been pressed</returns>
	 */
	bool is_key_pressed(BYTE key_code) const;
	/*
	 * <param name="key_code">win32 key code</param>
	 * <returns>Whether key code has been pressed this frame</returns>
	 */
	bool is_key_just_pressed(BYTE key_code) const;
};

inline gold_input gold_input;
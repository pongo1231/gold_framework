#pragma once

#define _NODISCARD [[nodiscard]]

using BYTE = unsigned char;

class gold_input
{
  private:
	bool key_states[255] {};
	bool key_prev_states[255] {};

  public:
	void run();

	_NODISCARD bool is_key_pressed(BYTE key_code) const;
	_NODISCARD bool is_key_just_pressed(BYTE key_code) const;
};

inline gold_input gold_input;
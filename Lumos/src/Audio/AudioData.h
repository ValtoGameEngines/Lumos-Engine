#pragma once

namespace Lumos
{
	struct AudioData
	{
		unsigned char*	Data;
		float	FreqRate = 0.0f;
		double	Length = 0.0;
		uint	BitRate = 0;
		uint	Size = 0;
		uint	Channels = 0;
	};
}
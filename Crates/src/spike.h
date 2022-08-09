struct Spike: Solid
{
	uint8_t damage = 1;
	uint8_t ttl = 0;

	Spike() = default;

	Spike(float const &X, float const &Y)
	{
		x = X, y = Y;
		width = 7, height = 8;
		cbox_conf = Rect(0, 4, 7, 4);
		type = 'S';

		ttl = 30;
	}

	void update()
	{
		if (arduboy.everyXFrames(60)) ttl--;

		updateCbox();
	}

	void draw() const
	{
		sketch(SPIKE);
	}
};

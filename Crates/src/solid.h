struct Solid: Entity
{
	char type = 'G'; // a generic type indicator
	uint8_t damage = 0;

	Rect cbox_conf = Rect(0, 0, width, height); // used for cbox offset and size
	Rect cbox; // used to actually calculate collisions

	void updateCbox()
	{
		// TODO does modifying like this allocate memory?
		cbox = Rect((int16_t)x + cbox_conf.x, (int16_t)y + cbox_conf.y, cbox_conf.width, cbox_conf.height);
	}
};

#include <stdlib.h>
#include <glib.h>
#include <clutter/clutter.h>
#include <clutter/x11/clutter-x11.h>

int
main(int argc, char *argv[])
{
	clutter_x11_set_use_argb_visual(TRUE);

	clutter_init(&argc, &argv);

	/* Create a new stage */
	ClutterActor *stage = clutter_stage_new();
	clutter_stage_set_title(CLUTTER_STAGE(stage), "JuShelf");
	clutter_stage_set_user_resizable(CLUTTER_STAGE(stage), FALSE);
	clutter_stage_set_use_alpha(CLUTTER_STAGE (stage), TRUE);
	clutter_actor_set_opacity(stage, 0x55);

	/* Quit when destory */
	g_signal_connect(stage, "destroy", G_CALLBACK(clutter_main_quit), NULL);

	/* Show All */
	clutter_actor_show_all(stage);

	/* Loop */
	clutter_main();

	return EXIT_SUCCESS;
}

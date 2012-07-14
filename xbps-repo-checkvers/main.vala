Checkvers chk;
Args args;

int
main(string[] argv)
{
	File d;
	chk = new Checkvers();
	args = new Args();

	args.parse(argv);

	if (chk.cfg == null) {
		stderr.printf("Error: You need to give a config or distdir!\n");
		args.usage();
	}

	if (chk.distdir == null)
		chk.config_get_distdir();

	d = File.new_for_path(chk.distdir);
	if (!d.query_exists()) {
		stderr.printf("ERROR: Can't find DISTDIR '%s'!\n", chk.distdir);
		return 1;
	}

	if (!d.get_child("srcpkgs").query_exists()) {
		stderr.printf("ERROR: '%s' doesn't appear to be ", chk.distdir);
		stderr.printf("a proper DISTDIR (no 'srcpkgs' subdir).\n");
		return 1;
	}

	chk.check_template_versions();

	return 0;
}

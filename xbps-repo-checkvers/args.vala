public class Args {

private OptionContext context;
private OptionGroup config_group;
private string utext;
public bool debug;

public
Args()
{
	debug = false;

	utext = """[TEMPLATES...]

	Copyright (c) 2012 The AUTHORS. See the AUTHORS file.
	See the COPYING file for license(s)/distribution details.""";

	config_group = new OptionGroup("config", "Configuration Options:",
	    "Show configuration options");
	
	create_entries();

	context = new OptionContext(utext);
	context.add_group((owned)config_group);
}

public void
create_entries()
{
	config_group.add_entries({
		OptionEntry() {
			long_name = "config", short_name = 'c',
			arg_data = &(chk.cfg), arg = OptionArg.FILENAME,
			description = "Override detected xbps-src config file",
			arg_description = "FILENAME"
		}, OptionEntry() {
			long_name = "distdir", short_name = 'd',
			arg_data = &(chk.distdir), arg = OptionArg.FILENAME,
			description = "Override xbps-src DISTDIR setting",
			arg_description = "DIRECTORY"
		}, OptionEntry()

	});
}

public void
parse(string[] argv)
{
	try {
		context.parse(ref argv);
	} catch (OptionError error) {
		stderr.printf("%s error: %s", "xbps-repo-checkvers",
			error.message);
		usage();
	}
	if (argv.length > 1) {
		chk.templates = new string[argv.length-1];
		for (int i = 1; i < argv.length; i++) {
			chk.templates[i-1] = argv[i];
		}
		return;
	}
}

public void
usage()
{
	stdout.printf("%s", context.get_help(false, null));
	Process.exit(1);
}


} /* class Args */

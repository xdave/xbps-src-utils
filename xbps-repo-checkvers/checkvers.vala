[CCode (cname="str_replace", cheader_filename="other.h")]
public extern string str_replace(string o, string p, string r);

const string[] cfgs = {
	"/etc/xbps/xbps-src.conf", "/usr/local/etc/xbps/xbps-src.conf"
};
const string[] v = { "XBPS_DISTDIR=", "$HOME" };
const string[] vrs = { "version", "revision" };
const string[] replacements = { "$", "{", "}", "\"", "'" };
const string ls_subpkgs = "/bin/sh -c \"/bin/ls -F %s|grep /\"";
const string output_fmt = "pkgname: %s repover %s srcpkgver: %s\n";
const string pkgpath_fmt = "%s/%s/template";

public class Checkvers {

public XBPS x;
public string cfg;
public string distdir;
public string[] templates;
public string pkgname;
public string repover;
public string version;
public string home;

public HashTable<string,string> ht;

public Regex varsplit;

public DateTime utc;
public MatchInfo info;

public
Checkvers()
{
	foreach (unowned string c in cfgs) {
		if(File.new_for_path(c).query_exists())
			cfg = c; break;
	}

	home = Environment.get_home_dir();
	utc = new DateTime.now_utc();
	ht = new HashTable<string,string>(str_hash, str_equal);
	x = new XBPS();
	try {
		varsplit = new Regex("^([A-z0-9_]+)=(.*?)$",
			RegexCompileFlags.MULTILINE);
	} catch (Error e) {
		stdout.printf("%s\n", e.message);
		Process.exit(1);
	}
}

public void
config_get_distdir()
{
	string line;
	long v0_len = v[0].length;
	long v1_len = v[1].length;

	try {
		var file = File.new_for_path(cfg);
		var dis = new DataInputStream(file.read());
		while ((line = dis.read_line(null)) != null) {
			if (v[0] in line) {
				line = line[v0_len:line.length];
				if (v[1] in line) {
					line = home + line[v1_len:line.length];
				}
				distdir = line;
				break;
			}
		}
	} catch (Error e) {
		stderr.printf("%s\n", e.message);
		stderr.printf("Couldn't read %s!\n", cfg);
		Process.exit(1);
	}
}

public void
check_template_versions()
{
	Dir dir;
	string srcpkgsdir;

	srcpkgsdir = distdir + "/srcpkgs";

	try {
		dir = Dir.open(srcpkgsdir, 0);
	} catch (FileError e) {
		stderr.printf("%s\n", e.message);
		Process.exit(1);
	}

	while((pkgname = dir.read_name()) != null) {
		process_pkg(srcpkgsdir, false);
	}

	try {
		for (int i = 0; i < templates.length; i++) {
			string path, n, tmpl;
			if ("srcpkgs" in templates[i]) {
				path = distdir + "/" + templates[i];
				if (!FileUtils.test(path, FileTest.EXISTS))
					break;
				n = templates[i].split("/")[1];
				tmpl = distdir + "/" + n;
				if (FileUtils.test(tmpl, FileTest.IS_SYMLINK))
					tmpl = FileUtils.read_link(tmpl);
				pkgname = tmpl[distdir.length+1:tmpl.length];
				process_pkg(srcpkgsdir, true);
			}
		}
	} catch (FileError e) {
		stderr.printf("%s\n", e.message);
		Process.exit(1);
	}
}

void
process_pkg(string srcpkgsdir, bool requested = false)
{
	string pkgpath, tmplpath;


	pkgpath = srcpkgsdir + "/" + pkgname;
	if (FileUtils.test(pkgpath, FileTest.IS_SYMLINK)) return;
	x.get_pkg_version(pkgname, out repover);

	if (repover == null) {
		if (requested == true) {
			stdout.printf("pkgname: %s repover: (NULL) ", pkgname);
			stdout.printf("[MANUAL]\n");
		} else {
			if (args.show_missing) {
				stdout.printf("%s\n", pkgname);
			} else { return; }
		}
	}
	if (repover != null && requested == false && !args.show_missing) {
		if (pkgname == "apache-mpm-event") return;
		if (pkgname == "apache-mpm-worker") return;
		tmplpath = pkgpath + "/template";
		version = get_template_version(tmplpath);
		if (xbps_cmpver(repover, version) == -1) {
			stdout.printf(output_fmt, pkgname, repover, version);
		}
		ht.remove_all();		
	}
}

public string
get_template_version(string srcpkg)
{
	string data, version;
	try {
		FileUtils.get_contents(srcpkg, out data);
		varsplit.match_full(data, -1, 0, 0, out info);
		while (info.matches()) {
			ht.set(info.fetch(1), info.fetch(2));
			info.next();
		}
		ht.for_each(replace_shell_vars<string, string>);

		for (int i = 0; i < replacements.length; i++) {
			hash_str_replace("version", replacements[i], "");
		}
		version = "%s_%s".printf(ht.get("version"), ht.get("revision"));
	} catch (Error e) {
		stderr.printf ("%s\n", e.message);
		Process.exit(1);
	}

	return version;
}

public void
replace_shell_vars(string key, string val)
{
	string version, date, datef;
	
	version = ht.get("version");
	date = """"$(date -u +%Y%m%d)"""";
	datef = "%Y%m%d";

	if (key[0] == '_' && !("$" in val)) {
		hash_str_replace("version", key, val);
	}

	if (version == date) {
		hash_str_replace("version", date, utc.format(datef));
	}
}

public void
hash_str_replace(string key, string pat, string repl)
{
	string v, new_v;
	v = ht.get(key);
	if (v != null) {
		new_v = str_replace(v, pat, repl);
		if (new_v != v) {
			ht.replace(key, new_v);
		}
	}
}

} /* class Checkvers */

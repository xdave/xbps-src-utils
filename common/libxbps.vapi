[Compact]
[CCode (cname="xbps", cheader_filename="libxbps.h", free_function="xhp_free", unref_function="")]
public class XBPS {
	[CCode (cname="xhp_init", cheader_filename="libxbps.h")]
	public XBPS();
	[CCode (cname="xhp_get_pkg_version", cheader_filename="libxbps.h")]
	public void get_pkg_version(string pkgname, out string repover);
}

[CCode (cname="xbps_cmpver", cheader_filename="xbps_api.h")]
public extern int xbps_cmpver(string a, string b);

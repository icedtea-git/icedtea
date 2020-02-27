package sun.tools.jar.resources;

import java.util.ListResourceBundle;

public final class jar_de extends ListResourceBundle {
    protected final Object[][] getContents() {
        return new Object[][] {
            { "error.bad.cflag", "Kennzeichen \"c\" erfordert Angabe von Manifest oder Eingabedateien." },
            { "error.bad.eflag", "Kennzeichen \"e\" und Manifest mit dem Attribut \"Main-Class\" k\u00F6nnen nicht zusammen angegeben\nwerden." },
            { "error.bad.option", "Eine der Optionen -{ctxu} muss angegeben werden." },
            { "error.bad.uflag", "Kennzeichen \"u\" erfordert Angabe von Manifest, Kennzeichen \"e\" oder Eingabedateien." },
            { "error.cant.open", "\u00D6ffnen nicht m\u00F6glich: {0} " },
            { "error.create.dir", "{0}: Verzeichnis konnte nicht erstellt werden" },
            { "error.illegal.option", "Unzul\u00E4ssige Option: {0}" },
            { "error.incorrect.length", "Falsche L\u00E4nge bei der Verarbeitung: {0}" },
            { "error.nosuch.fileordir", "{0}: Datei oder Verzeichnis nicht vorhanden" },
            { "error.write.file", "Fehler beim Schreiben in vorhandener JAR-Datei" },
            { "out.added.manifest", "Manifest wurde hinzugef\u00FCgt" },
            { "out.adding", "{0} wird hinzugef\u00FCgt" },
            { "out.create", "  erstellt: {0}" },
            { "out.deflated", "({0} % verkleinert)" },
            { "out.extracted", "extrahiert: {0}" },
            { "out.ignore.entry", "Eintrag {0} wird ignoriert" },
            { "out.inflated", " vergr\u00F6\u00DFert: {0}" },
            { "out.size", "(ein = {0}) (aus = {1})" },
            { "out.stored", "(0 % gespeichert)" },
            { "out.update.manifest", "Manifest wurde aktualisiert" },
            { "usage", "Verwendung: jar {ctxui}[vfm0PMe] [jar-file] [manifest-file] [entry-point] [-C dir] Dateien...\nOptionen:\n    -c  Neues Archiv erstellen\n    -t  Inhaltsverzeichnis f\u00FCr Archiv anzeigen\n    -x  Benannte (oder alle) Dateien aus dem Archiv extrahieren\n    -u  Vorhandenes Archiv aktualisieren\n    -v  Ausgabe im Verbose-Modus aus Standard-Ausgabe generieren\n    -f  Dateinamen f\u00FCr Archiv angeben\n    -m  Manifestinformationen aus angegebener Manifestdatei einschlie\u00DFen\n    -e  Anwendungseinstiegspunkt f\u00FCr Standalone-Anwendung angeben \n        in einer ausf\u00FChrbaren JAR-Datei geb\u00FCndelt\n    -0  Nur speichern; keine ZIP-Komprimierung verwenden\n    -P  Komponenten mit vorangestelltem \"/\" (absoluter Pfad) und \"..\" (\u00FCbergeordnetes Verzeichnis) aus Dateinamen beibehalten\n    -M  Keine Manifest-Datei f\u00FCr die Eintr\u00E4ge erstellen\n    -i  Indexinformationen f\u00FCr die angegebenen JAR-Dateien erstellen\n    -C  Zum angegebenen Verzeichnis wechseln und folgende Datei einschlie\u00DFen\nFalls eine Datei ein Verzeichnis ist, wird dieses rekursiv verarbeitet.\nDer Name der Manifestdatei, der Name der Archivdatei und der Name des Einstiegspunkts werden\nin derselben Reihenfolge wie die Kennzeichen \"m\", \"f\" und \"e\" angegeben.\n\nBeispiel 1: Archivieren Sie zwei Klassendateien in ein Archiv mit Namen \"classes.jar\": \n       jar cvf classes.jar Foo.class Bar.class \nBeispiel 2: Verwenden Sie die vorhandenen Manifestdatei \"mymanifest\", und archivieren Sie alle\n           Dateien im Verzeichnis foo/ directory in \"classes.jar\": \n       jar cvfm classes.jar mymanifest -C foo/ .\n" },
        };
    }
}

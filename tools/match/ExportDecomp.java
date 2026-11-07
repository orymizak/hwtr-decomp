// Exporta a un JSONL firma y pseudocódigo de todas las funciones. Argumento: ruta de salida.
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Parameter;
import ghidra.program.model.pcode.HighFunction;
import ghidra.program.model.pcode.HighFunctionDBUtil;
import java.io.PrintWriter;

public class ExportDecomp extends GhidraScript {
    private static String json(String s) {
        StringBuilder b = new StringBuilder("\"");
        for (char c : s.toCharArray()) {
            if (c == '"' || c == '\\') {
                b.append('\\').append(c);
            } else if (c == '\n') {
                b.append("\\n");
            } else if (c < 0x20) {
                b.append(' ');
            } else {
                b.append(c);
            }
        }
        return b.append('"').toString();
    }

    @Override
    public void run() throws Exception {
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        int ok = 0, bad = 0;
        try (PrintWriter out = new PrintWriter(getScriptArgs()[0], "UTF-8")) {
            for (Function f : currentProgram.getFunctionManager().getFunctions(true)) {
                if (monitor.isCancelled()) {
                    break;
                }
                DecompileResults r = decomp.decompileFunction(f, 60, monitor);
                if (!r.decompileCompleted()) {
                    bad++;
                    continue;
                }
                HighFunction h = r.getHighFunction();
                StringBuilder params = new StringBuilder("[");
                int n = h.getFunctionPrototype().getNumParams();
                for (int i = 0; i < n; i++) {
                    params.append(i == 0 ? "" : ",").append(json(h.getFunctionPrototype().getParam(i).getDataType().getName()));
                }
                params.append("]");
                out.println("{\"name\":" + json(f.getName()) + ",\"addr\":" + json(f.getEntryPoint().toString())
                    + ",\"ret\":" + json(h.getFunctionPrototype().getReturnType().getName())
                    + ",\"params\":" + params + ",\"c\":" + json(r.getDecompiledFunction().getC()) + "}");
                ok++;
            }
        }
        println("EXPORTADAS " + ok + " FALLOS " + bad + " LENGUAJE " + currentProgram.getLanguageID());
    }
}

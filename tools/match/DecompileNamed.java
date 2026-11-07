// Imprime el pseudocódigo de las funciones pedidas por nombre.
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;

public class DecompileNamed extends GhidraScript {
    @Override
    public void run() throws Exception {
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);
        println("PROCESADOR " + currentProgram.getLanguageID() + " FUNCIONES " + currentProgram.getFunctionManager().getFunctionCount());
        for (String name : getScriptArgs()) {
            Function found = null;
            for (Function f : currentProgram.getFunctionManager().getFunctions(true)) {
                if (f.getName().equals(name)) {
                    found = f;
                    break;
                }
            }
            if (found == null) {
                println("NO ENCONTRADA " + name);
                continue;
            }
            DecompileResults res = decomp.decompileFunction(found, 60, monitor);
            println("=== " + name + " @ " + found.getEntryPoint() + " refs=" + found.getSymbol().getReferenceCount());
            println(res.decompileCompleted() ? res.getDecompiledFunction().getC() : "ERROR " + res.getErrorMessage());
        }
    }
}

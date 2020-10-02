/* ###
 * IP: GHIDRA
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
// An example script demonstrating the ability to emulate a specific portion of code within
// a disassembled program to extract return values of interest (deobfuscated data in this case)
// and generate program listing comments.
// This script emulates the "main" function within the deobExample program
// (see docs/GhidraClass/ExerciseFiles/Emulation/Source) built with gcc for x86-64.
// The program's "data" array contains simple obfuscated data and has a function "deobfuscate"
// which is called for each piece of obfuscated data.  The "main" function loops through all
// the data and deobfuscates each one invoking the "use_string" function for each deobfuscated
// data.  Breakpoints are placed on the call (and just after the call)
// to the function "deobfuscate" so that the various return values can be recorded with a comment
// placed just after the call.
//@category Examples.Emulation
import ghidra.app.emulator.EmulatorHelper;
import ghidra.app.script.GhidraScript;
import ghidra.app.util.opinion.ElfLoader;
import ghidra.pcode.emulate.EmulateExecutionState;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.Program;
import ghidra.program.model.symbol.*;
import ghidra.util.Msg;
import ghidra.util.exception.NotFoundException;

public class EmuX86DeobfuscateExampleScript extends GhidraScript {

	private static String PROGRAM_NAME = "cs";

	private EmulatorHelper emuHelper;

	// Important breakpoint locations
	private Address deobfuscateCall;
	private Address deobfuscateReturn;

	// Function locations
	private Address mainFunctionEntry; // start of emulation address

	// Address used as final return location
	// A breakpoint will be set here so we can determine when function execution
	// has completed.
	private static final long CONTROLLED_RETURN_OFFSET = 0;
	private Address controlledReturnAddr; // end of emulation address

	// First argument passed to deobfuscate function on last call (used for comment generation)
	private long lastDeobfuscateArg0;

	@Override
	protected void run() throws Exception {

		String format =
			currentProgram.getOptions(Program.PROGRAM_INFO).getString("Executable Format", null);

		if (currentProgram == null || !currentProgram.getName().startsWith(PROGRAM_NAME) ||
			!"x86:LE:32:default".equals(currentProgram.getLanguageID().toString()) ||
			!ElfLoader.ELF_NAME.equals(format)) {

			printerr(
				"This emulation example script is specifically intended to be executed against the\n" +
					PROGRAM_NAME +
					" program whose source is contained within the GhidraClass exercise files\n" +
					"(see docs/GhidraClass/ExerciseFiles/Emulation/" + PROGRAM_NAME + ".c).\n" +
					"This program should be compiled using gcc for x86 32-bit, imported into your project, \n" +
					"analyzed and open as the active program before running ths script.");
			return;
		}

		// Identify function to be emulated
		//mainFunctionEntry = getSymbolAddress("main");
		//mainFunctionEntry = getSymbolAddress("CS1_Latin40_EncodeFilename");
		mainFunctionEntry = getSymbolAddress("CS1_Latin40_DecodeFilename");

		// Obtain entry instruction in order to establish initial processor context
		Instruction entryInstr = getInstructionAt(mainFunctionEntry);
		if (entryInstr == null) {
			printerr("Instruction not found at main entry point: " + mainFunctionEntry);
			return;
		}

		// Identify important symbol addresses
		// NOTE: If the sample is recompiled the following addresses may need to be adjusted
		/*Instruction callSite = getCalledFromInstruction("deobfuscate");
		if (callSite == null) {
			printerr("Instruction not found at call site for: deobfuscate");
			return;
		}*/

		//deobfuscateCall = callSite.getAddress();
		//deobfuscateReturn = callSite.getFallThrough(); // instruction address immediately after deobfuscate call

		// Remove prior pre-comment
		//setPreComment(deobfuscateReturn, null);

		// Establish emulation helper
		emuHelper = new EmulatorHelper(currentProgram);
		try {
			long thisSize = 0x240;
			int thisRelative = 32;
			byte[] advData = fromHexString("00 A0 65 45 00 00 00 00 25 02 00 00 00 00 85 CF 00 00 00 00 D5 8A 65 45");
			byte[] startData = fromHexString("B0 D1 9F B5 90 6C 0D 00 F9 AB 92 B5");
			byte[] advText = fromHexString("61 64 76 00 00 00 00 00 00 00 00 00 00 00 00 00 63 73 78 00 00 00 00 00 00 00 00 00 00 00");
			byte[] startText = fromHexString("73 74 61 72 74 00 00 00 63 73 78 00");
			String advString = "adv.csx";
			String startString = "start.csx";
			byte[] advStringData = fromHexString("61 64 76 2E 63 73 78");
			byte[] startStringData = fromHexString("73 74 61 72 74 2E 63 73 78");
			int paramBufferRelative = thisRelative+32;
			int paramTextRelative   = paramBufferRelative+64;
			int field0x220 = 0x18;
			int field0x224 = 0x1e;

			// Initialize stack pointer (not used by this example)
			long stackOffset =
				(entryInstr.getAddress().getAddressSpace().getMaxAddress().getOffset() >>> 1) -
					0x7fff;
			stackOffset = stackOffset - (thisRelative+thisSize);//thisSize - 16;
			emuHelper.writeRegister(emuHelper.getStackPointerRegister(), stackOffset);
			//emuHelper.writeRegister("EBP", stackOffset);
			//emuHelper.writeRegister("ESP", stackOffset);

			// ZERO STACK:
			writeStack(thisRelative, new byte[thisSize]);

			// INIT STACK DATA:
			writeStack(paramTextRelative, new byte[64]); // uint->[64] (param_1)
			writeStack(paramBufferRelative, new byte[64]); // uint->[64] (param_2)
			//writeStack(paramTextRelative, advStringData); // uint->[64] (param_1)
			writeStack(paramBufferRelative, advData); // uint->[64] (param_2)
			emuHelper.writeStackValue(thisRelative + 0x220, 4, field0x220); // uint this->0x220 (entrysize)
			emuHelper.writeStackValue(thisRelative + 0x224, 4, field0x224); // uint this->0x224 (textsize, [decoded])

			//emuHelper.getProgram().getCompilerSpec().getStackBaseSpace();
			
			// INIT STACK PARAMS:
			emuHelper.writeRegister("ECX", stackOffset + thisRelative); // EXC: void *this
			emuHelper.writeStackValue(0x4, 4, stackOffset + paramTextRelative); // param_1 char *text
			emuHelper.writeStackValue(0x8, 4, stackOffset + paramBufferRelative); // param_2 uint *buffer

			// Setup breakpoints
			//emuHelper.setBreakpoint(deobfuscateCall);
			//emuHelper.setBreakpoint(deobfuscateReturn);

			// Set controlled return location so we can identify return from emulated function
			controlledReturnAddr = getAddress(CONTROLLED_RETURN_OFFSET);
			//emuHelper.writeStackValue(0, 8, CONTROLLED_RETURN_OFFSET);
			emuHelper.writeStackValue(0x0, 4, CONTROLLED_RETURN_OFFSET);
			emuHelper.setBreakpoint(controlledReturnAddr);

			Msg.debug(this, "EMU starting at " + mainFunctionEntry);

			// Execution loop until return from function or error occurs
			while (!monitor.isCancelled()) {
				boolean success =
					(emuHelper.getEmulateExecutionState() == EmulateExecutionState.BREAKPOINT)
							? emuHelper.run(monitor)
							: emuHelper.run(mainFunctionEntry, entryInstr, monitor);
				Address executionAddress = emuHelper.getExecutionAddress();
				if (monitor.isCancelled()) {
					println("Emulation cancelled");
					return;
				}
				if (executionAddress.equals(controlledReturnAddr)) {
					println("Returned from function");
					println("char *param_1 (text output)");
					printHexDump(readStackAt(paramTextRelative), 0);
					return;
				}
				if (!success) {
					String lastError = emuHelper.getLastError();
					printerr("Emulation Error: " + lastError);
					return;
				}
				processBreakpoint(executionAddress);
			}
		}
		finally {
			// cleanup resources and release hold on currentProgram
			emuHelper.dispose();
		}
	}
	public byte[] readStack(int relativeOffset, int size)
			throws Exception {
		long offset = readRegister(stackPtrReg).longValue() + relativeOffset;
		byte[] bytes = readMemory(emuHelper.getProgram().getCompilerSpec().getStackBaseSpace().getAddress(offset), size);
		return bytes;
	}
	public byte[] readStackAt(long offset, int size)
			throws Exception {
		byte[] bytes = readMemory(emuHelper.getProgram().getCompilerSpec().getStackBaseSpace().getAddress(offset), size);
		return bytes;
	}
	private void writeStack(int relativeOffset, byte[] bytes) throws Exception {
		long offset = readRegister(stackPtrReg).longValue() + relativeOffset;
		writeMemory(emuHelper.getProgram().getCompilerSpec().getStackBaseSpace().getAddress(offset), bytes);
	}

	private Address getAddress(long offset) {
		return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(offset);
	}
	private void printHexDump(byte[] a, int visualOffset) {
		//StringBuilder sb = new StringBuilder(a.length * 2);
		//int index = 0;
		//8+1+16*3+4+16
		int len = a.length();
		println("  Offset: 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F   ");
		for (int i = 0; i < len; i += 16) {
			StringBuilder sb = new StringBuilder(8+1+16*3+4+16  +32); // miscalc extra
			sb.append(String.format("%08X", visualOffset+i));
			sb.append(':');
			for (int j = 0; j < 16 && i+j < len; j++) {
				sb.append(' ');
				sb.append(String.format("%02X", a[i+j]));
			}
			sb.append("    ");
			for (int j = 0; j < 16 && i+j < len; j++) {
				if (a[i+j] >= 0x20 && a[i+j] <= 0x7E)
					sb.append((char) a[i+j]);
				else
					sb.append('.');
			}
			println(sb.toString());
		}
	}
	/**
	 * Perform processing for the various breakpoints.
	 * @param addr current execute address where emulation has been suspended
	 * @throws Exception if an error occurs
	 */
	private void processBreakpoint(Address addr) 
		//throws Exception
	{

		// if (addr.equals(deobfuscateCall)) {
		// 	lastDeobfuscateArg0 = emuHelper.readRegister("RDI").longValue();
		// }

		// else if (addr.equals(deobfuscateReturn)) {
		// 	long deobfuscateReturnValue = emuHelper.readRegister("RAX").longValue();
		// 	String str = "deobfuscate(src=0x" + Long.toHexString(lastDeobfuscateArg0) + ") -> \"" +
		// 		emuHelper.readNullTerminatedString(getAddress(deobfuscateReturnValue), 32) + "\"";
		// 	String comment = getPreComment(deobfuscateReturn);
		// 	if (comment == null) {
		// 		comment = "";
		// 	}
		// 	else {
		// 		comment += "\n";
		// 	}
		// 	comment += str;
		// 	println("Updated pre-comment at " + deobfuscateReturn);
		// 	setPreComment(deobfuscateReturn, comment);
		// }
	}

	private byte[] fromHexString(String src) {
		src = src.replaceAll("\\s", "");
		int len = src.length();
		byte[] data = new byte[len / 2];
		for (int i = 0; i < len; i += 2) {
			data[i / 2] = (byte) ((Character.digit(src.charAt(i), 16) << 4)
								+ Character.digit(src.charAt(i+1), 16));
		}
		return data;
		// byte[] biBytes = new BigInteger("10" + src.replaceAll("\\s", ""), 16).toByteArray();
		// return Arrays.copyOfRange(biBytes, 1, biBytes.length);
	}
	// private byte[] hexStringToByteArray(String s) {
	// 	int len = s.length();
	// 	byte[] data = new byte[len / 2];
	// 	for (int i = 0; i < len; i += 2) {
	// 		data[i / 2] = (byte) ((Character.digit(s.charAt(i), 16) << 4)
	// 							+ Character.digit(s.charAt(i+1), 16));
	// 	}
	// 	return data;
	// }

	private Instruction getCalledFromInstruction(String functionName) {
		Symbol s = SymbolUtilities.getExpectedLabelOrFunctionSymbol(currentProgram, functionName,
			m -> printerr(m));
		for (Reference ref : s.getReferences(monitor)) {
			if (ref.getReferenceType().isCall()) {
				return currentProgram.getListing().getInstructionAt(ref.getFromAddress());
			}
		}
		return null;
	}

	private Address getSymbolAddress(String symbolName) throws NotFoundException {
		Symbol symbol = SymbolUtilities.getLabelOrFunctionSymbol(currentProgram, symbolName,
			err -> Msg.error(this, err));
		if (symbol != null) {
			return symbol.getAddress();
		}
		throw new NotFoundException("Failed to locate label: " + symbolName);
	}

}

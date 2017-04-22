//Ethan Clark <ejc49@zips.uakron.edu>

#include "cpu.h"


modeEnum::sourceMode modes[modeEnum::siz] = {
	modeEnum::imp, modeEnum::xid, modeEnum::imp, modeEnum::xid,
	modeEnum::zop, modeEnum::zop, modeEnum::zop, modeEnum::zop,
	modeEnum::imp, modeEnum::imm, modeEnum::acc, modeEnum::imm,
	modeEnum::abs, modeEnum::abs, modeEnum::abs, modeEnum::abs,
	modeEnum::rel, modeEnum::idx, modeEnum::imp, modeEnum::idx,
	modeEnum::zpx, modeEnum::zpx, modeEnum::zpx, modeEnum::zpx,
	modeEnum::imp, modeEnum::aby, modeEnum::imp, modeEnum::aby,
	modeEnum::abx, modeEnum::abx, modeEnum::abx, modeEnum::abx,
	modeEnum::abs, modeEnum::xid, modeEnum::imp, modeEnum::xid,
	modeEnum::zop, modeEnum::zop, modeEnum::zop, modeEnum::zop,
	modeEnum::imp, modeEnum::imm, modeEnum::acc, modeEnum::imm,
	modeEnum::abs, modeEnum::abs, modeEnum::abs, modeEnum::abs,
	modeEnum::rel, modeEnum::idx, modeEnum::imp, modeEnum::idx,
	modeEnum::zpx, modeEnum::zpx, modeEnum::zpx, modeEnum::zpx,
	modeEnum::imp, modeEnum::aby, modeEnum::imp, modeEnum::aby,
	modeEnum::abx, modeEnum::abx, modeEnum::abx, modeEnum::abx,
	modeEnum::imp, modeEnum::xid, modeEnum::imp, modeEnum::xid,
	modeEnum::zop, modeEnum::zop, modeEnum::zop, modeEnum::zop,
	modeEnum::imp, modeEnum::imm, modeEnum::acc, modeEnum::imm,
	modeEnum::abs, modeEnum::abs, modeEnum::abs, modeEnum::abs,
	modeEnum::rel, modeEnum::idx, modeEnum::imp, modeEnum::idx,
	modeEnum::zpx, modeEnum::zpx, modeEnum::zpx, modeEnum::zpx,
	modeEnum::imp, modeEnum::aby, modeEnum::imp, modeEnum::aby,
	modeEnum::abx, modeEnum::abx, modeEnum::abx, modeEnum::abx,
	modeEnum::imp, modeEnum::xid, modeEnum::imp, modeEnum::xid,
	modeEnum::zop, modeEnum::zop, modeEnum::zop, modeEnum::zop,
	modeEnum::imp, modeEnum::imm, modeEnum::acc, modeEnum::imm,
	modeEnum::ind, modeEnum::abs, modeEnum::abs, modeEnum::abs,
	modeEnum::rel, modeEnum::idx, modeEnum::imp, modeEnum::idx,
	modeEnum::zpx, modeEnum::zpx, modeEnum::zpx, modeEnum::zpx,
	modeEnum::imp, modeEnum::aby, modeEnum::imp, modeEnum::aby,
	modeEnum::abx, modeEnum::abx, modeEnum::abx, modeEnum::abx,
	modeEnum::imm, modeEnum::xid, modeEnum::imm, modeEnum::xid,
	modeEnum::zop, modeEnum::zop, modeEnum::zop, modeEnum::zop,
	modeEnum::imp, modeEnum::imm, modeEnum::imp, modeEnum::imm,
	modeEnum::abs, modeEnum::abs, modeEnum::abs, modeEnum::abs,
	modeEnum::rel, modeEnum::idx, modeEnum::imp, modeEnum::idx,
	modeEnum::zpx, modeEnum::zpx, modeEnum::zpy, modeEnum::zpy,
	modeEnum::imp, modeEnum::aby, modeEnum::imp, modeEnum::aby,
	modeEnum::abx, modeEnum::abx, modeEnum::aby, modeEnum::aby,
	modeEnum::imm, modeEnum::xid, modeEnum::imm, modeEnum::xid,
	modeEnum::zop, modeEnum::zop, modeEnum::zop, modeEnum::zop,
	modeEnum::imp, modeEnum::imm, modeEnum::imp, modeEnum::imm,
	modeEnum::abs, modeEnum::abs, modeEnum::abs, modeEnum::abs,
	modeEnum::rel, modeEnum::idx, modeEnum::imp, modeEnum::idx,
	modeEnum::zpx, modeEnum::zpx, modeEnum::zpy, modeEnum::zpy,
	modeEnum::imp, modeEnum::aby, modeEnum::imp, modeEnum::aby,
	modeEnum::abx, modeEnum::abx, modeEnum::aby, modeEnum::aby,
	modeEnum::imm, modeEnum::xid, modeEnum::imm, modeEnum::xid,
	modeEnum::zop, modeEnum::zop, modeEnum::zop, modeEnum::zop,
	modeEnum::imp, modeEnum::imm, modeEnum::imp, modeEnum::imm,
	modeEnum::abs, modeEnum::abs, modeEnum::abs, modeEnum::abs,
	modeEnum::rel, modeEnum::idx, modeEnum::imp, modeEnum::idx,
	modeEnum::zpx, modeEnum::zpx, modeEnum::zpx, modeEnum::zpx,
	modeEnum::imp, modeEnum::aby, modeEnum::imp, modeEnum::aby,
	modeEnum::abx, modeEnum::abx, modeEnum::abx, modeEnum::abx,
	modeEnum::imm, modeEnum::xid, modeEnum::imm, modeEnum::xid,
	modeEnum::zop, modeEnum::zop, modeEnum::zop, modeEnum::zop,
	modeEnum::imp, modeEnum::imm, modeEnum::imp, modeEnum::imm,
	modeEnum::abs, modeEnum::abs, modeEnum::abs, modeEnum::abs,
	modeEnum::rel, modeEnum::idx, modeEnum::imp, modeEnum::idx,
	modeEnum::zpx, modeEnum::zpx, modeEnum::zpx, modeEnum::zpx,
	modeEnum::imp, modeEnum::aby, modeEnum::imp, modeEnum::aby,
	modeEnum::abx, modeEnum::abx, modeEnum::abx, modeEnum::abx
};
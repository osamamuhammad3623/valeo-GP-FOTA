local function DSC_req_handle(buffer, oSubTree)
	local iProtocolDataLength = buffer:len()
	if iProtocolDataLength > 2 then
		oSubTree:add(buffer(2), 'Protocol error', 'unexpected data')
	else 
		local oTvbSubFunction = buffer(1, 1)
		oSubTree:add(oProto.fields.subfunction, oTvbSubFunction)
	end
end

local function SA_req_handle(buffer, oSubTree)
end

local function RC_req_handle(buffer, oSubTree)
end

local function RD_req_handle(buffer, oSubTree)
end

local function TD_req_handle(buffer, oSubTree)
end

local function RTE_req_handle(buffer, oSubTree)
	local iProtocolDataLength = buffer:len()
	if iProtocolDataLength > 1 then
		oSubTree:add(buffer(1), 'Protocol error', 'unexpected data')
	end
end

local function ER_req_handle(buffer, oSubTree)
end

local function DSC_response_handle(buffer, oSubTree)
end

local function SA_response_handle(buffer, oSubTree)
end

local function RC_response_handle(buffer, oSubTree)
end

local function RD_response_handle(buffer, oSubTree)
end

local function TD_response_handle(buffer, oSubTree)
end

local function RTE_response_handle(buffer, oSubTree)
end

local function ER_response_handle(buffer, oSubTree)
end

local function NR_handle(buffer, oSubTree)
end

local function DefineAndRegisterUDSdissector()
	local sProtocol = 'fota uds'
	local oProto = Proto(sProtocol, sProtocol:upper() .. ' Protocol')
	
	local DSC_req = 0X10, DSC_response = 0x50
	local SA_req = 0x27, SA_response = 0x67
	local RC_req = 0x31, RC_response = 0x71
	local RD_req = 0x34, RD_response = 0x74
	local TD_req = 0x36, TD_response = 0x76
	local RTE_req = 0x37, RTE_response = 0x77
	local ER_req = 0x11, ER_response = 0x51
	local NR = 0x7F

	local tService = {
		[DSC_req]='Diagnostic Session Control', [SA_req]='Security Access', [RC_req]='Routine Control',
		[RD_req]='Request Download', [TD_req]='Transfer Data', [RTE_req]='Request Transfer Exit', [ER_req]='ECU Reset',
    	[DSC_response]='Diagnostic Session Control Positive Response', [SA_response]='Security Access Positive Response', 
		[RC_response]='Routine Control Positive Response', [RD_response]='Request Download Positive Response', 
		[TD_response]='Transfer Data Positive Response', [RTE_response]='Request Transfer Exit Positive Response', 
		[ER_response]='ECU Reset Positive Response', [NR]='Negative Response'
	}

	local func_table = {
		[DSC_req]='DSC_req_handle', [SA_req]='SA_req_handle', [RC_req]='RC_req_handle',
		[RD_req]='RD_req_handle', [TD_req]='TD_req_handle', [RTE_req]='RTE_req_handle', [ER_req]='ER_req_handle',
    	[DSC_response]='DSC_response_handle', [SA_response]='SA_response_handle', [RC_response]='RC_response_handle', 
		[RD_response]='RD_response_handle', [TD_response]='TD_response_handle', [RTE_response]='RTE_response_handle', 
		[ER_response]='ER_response_handle', [NR]='NR_handle'
	}

	local DSC_EXT_subfunction = 0x03, SA_RS_subfunction = 0x01, SA_SK_subfunction = 0x02
	local RC_SR_subfunction = 0x01, ER_SR_subfunction = 0x03

	--can there be same index value in hex!
	local tSubFunction = {
		[DSC_EXT_subfunction]='Extended Session', [SA_RS_subfunction]='Request Seed', [SA_SK_subfunction]='Send Key',
		[RC_SR_subfunction]='Start Routine', [ER_SR_subfunction]='Soft Reset'
	}

	local RC_erase_memory = 0xFF00, RC_check_memory = 0x0202

	local tRoutineID = {
		[RC_erase_memory]='Erase Memory', [RC_check_memory]='Check Memory'
	}

	local SNS_NRC = 0x11, SFNS_NRC = 0x12, COC_NRC = 0x22
	local RSE_NRC = 0x24, ROOR_NRC = 0x31, SAD_NRC = 0x33
	local IK_NRC = 0x35, ENOA_NRC = 0x36, PR_NRC = 0x78

	local tNRC = {
		[SNS_NRC]='Service Not Supported', [SFNS_NRC]='SubFunction Not Supported', [COC_NRC]='Conditions Not Correct',
		[RSE_NRC]='Request Sequence Error', [ROOR_NRC]='Request Out Of Range', [SAD_NRC]='Security Access Denied',
		[IK_NRC]='Invalid Key', [ENOA_NRC]='Exceeded Number Of Attempts', [PR_NRC]='Pending Response'
	}

	oProto.fields.service = ProtoField.uint8(sProtocol .. '.service', 'Service', base.hex, tService, nil, 'The service')
	oProto.fields.subfunction = ProtoField.uint8(sProtocol .. '.subfunction', 'SubFunction', base.hex, tSubFunction, nil, 'The subfunction of service')
	oProto.fields.routineId = ProtoField.uint16(sProtocol .. '.routineId', 'RoutineId', base.hex, tRoutineID, nil, 'The routine id of service')
	oProto.fields.seed = ProtoField.uint32(sProtocol .. '.seed', 'Seed', base.hex, nil, nil, 'The requested seed')
	oProto.fields.key = ProtoField.string(sProtocol .. '.key', 'Key', 'The generated key')
	oProto.fields.downloadSize = ProtoField.uint32(sProtocol .. '.downloadSize', 'DownloadSize', base.DEC, nil, nil, 'The download size')
	oProto.fields.chunkSize = ProtoField.uint16(sProtocol .. '.chunkSize', 'ChunkSize', base.DEC, nil, nil, 'The chunk size')
	oProto.fields.data = ProtoField.string(sProtocol .. '.data', 'Data', 'The transferred data')
	oProto.fields.crc = ProtoField.uint32(sProtocol .. '.crc', 'CRC', base.hex, nil, nil, 'The crc')
	--add a separate tRequestedService!
	oProto.fields.requestedService = ProtoField.uint8(sProtocol .. '.requestedService', 'RequestedService', base.hex, tService, nil, 'The requested service')	
	oProto.fields.nrc = ProtoField.uint8(sProtocol .. '.nrc', 'NRC', base.hex, tNRC, nil, 'The negative response code')

	function oProto.dissector(buffer, oPinfo, oTreeItemRoot)
    	local iProtocolDataLength = buffer:len()
    	local oTvbService = buffer(0, 1)
    	local uiService = oTvbService:uint()

		-- We expect a valid service id, otherwise we don't dissect
		if tService[uiService] == nil then
			return
		end
        
    	oPinfo.cols.protocol = oProto.name
		oPinfo.cols.info = tService[uiService]

		local oSubTree = oTreeItemRoot:add(oProto, buffer())
		oSubTree:add(oProto.fields.service, oTvbService)

		--check needed parameters to be passed or put functions inside! 
		local func = func_table[uiService]
		func(buffer, oSubTree)
	end

	DissectorTable.get('tcp.port'):add(5005, oProto)    --change port number!
end

local function Main()
	DefineAndRegisterUDSdissector()
end

Main()

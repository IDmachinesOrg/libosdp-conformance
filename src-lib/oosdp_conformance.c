/*
  oosdp_conformance - conformance reporting routines

  (C)2014-2017 Smithee Spelvin Agnew & Plinge, Inc.

  Support provided by the Security Industry Association
  http://www.securityindustry.org

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at
 
    http://www.apache.org/licenses/LICENSE-2.0
 
  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/


#include <stdio.h>
#include <string.h>
//#include <time.h>
//#include <arpa/inet.h>


//#include <osdp-tls.h>
#include <open-osdp.h>
#include <osdp_conformance.h>


extern OSDP_INTEROP_ASSESSMENT
  osdp_conformance;
char
  log_string [1024];

// code to configure tests to skip
#include <osdp-SKIP.c>


char
  *conformance_status
    (unsigned char
      cstat)

{ /* conformance_status */

  static char
    response [1024];

  switch (cstat)
  {
  case OCONFORM_SKIP:
    strcpy (response, "Skipped");
    osdp_conformance.skipped ++;
    break;
  case OCONFORM_UNTESTED:
    strcpy (response, "Untested");
    osdp_conformance.untested ++;
    break;
  case OCONFORM_EXERCISED:
    strcpy (response, "Exercised");
    osdp_conformance.pass ++;
    break;
  case OCONFORM_EX_GOOD_ONLY:
    strcpy (response, "Exercised (no edge case tests)");
    osdp_conformance.pass ++;
    break;
  case OCONFORM_FAIL:
    strcpy (response, "Failed");
    osdp_conformance.fail ++;
    break;
  default:
    sprintf (response, "conformance status unknown(%d)", cstat);
    break;
  };
  return (response);

} /* conformance_status */


void
  dump_conformance
    (OSDP_CONTEXT *ctx,
    OSDP_INTEROP_ASSESSMENT *oconf)

{ /* dump_conformance */

  oconf->pass = 0;
  oconf->fail = 0;
  oconf->untested = 0;
  oconf->skipped = 0;

  skip_conformance_tests (ctx, oconf);

  // fill in results for "minimal message threshhold" case

  if (oconf->conforming_messages >= PARAM_MMT)
  {
    oconf->physical_interface.test_status =
      OCONFORM_EXERCISED;
    if (0 EQUALS strcmp (ctx->serial_speed, "9600"))
      oconf->signalling.test_status =
        OCONFORM_EXERCISED;
    oconf->packet_format.test_status =
      OCONFORM_EXERCISED;
    oconf->SOM.test_status =
      OCONFORM_EXERCISED;
    oconf->SOM_sent.test_status =
      OCONFORM_EXERCISED;
    oconf->LEN.test_status =
      OCONFORM_EXERCISED;
    oconf->CTRL.test_status =
      OCONFORM_EXERCISED;
    oconf->CTRL.test_status =
      OCONFORM_EXERCISED;
    oconf->CHKSUM_CRC16.test_status =
      OCONFORM_EXERCISED;
  };

#define LOG_REPORT(lfargs) {\
  sprintf lfargs; \
  fprintf (ctx->log, "%s\n", log_string); fflush (ctx->log); \
  fprintf (ctx->report, "%s\n", log_string); fflush (ctx->report);\
}; 

  ctx->report = fopen ("/opt/osdp-conformance/log/report.log", "w");

  LOG_REPORT ((log_string, "Conformance Report:"));

  LOG_REPORT ((log_string,
"2-1-1  Physical Interface                 %s",
    conformance_status (oconf->physical_interface.test_status)));
  LOG_REPORT ((log_string,
"2-2-1  Signalling (9600)                  %s",
    conformance_status (oconf->signalling.test_status)));
  LOG_REPORT ((log_string,
"2-2-2  Signalling (19200)                 %s",
    conformance_status (oconf->alt_speed_2.test_status)));
  LOG_REPORT ((log_string,
"2-2-3  Signalling (38400)                 %s",
    conformance_status (oconf->alt_speed_3.test_status)));
  LOG_REPORT ((log_string,
"2-2-4  Signalling (115200)                %s",
    conformance_status (oconf->alt_speed_4.test_status)));
  LOG_REPORT ((log_string,
"2-3-1  Character Encoding                 %s",
    conformance_status (oconf->character_encoding.test_status)));
  LOG_REPORT ((log_string,
"2-4-1  Channel Access                     %s",
    conformance_status (oconf->channel_access.test_status)));
  LOG_REPORT ((log_string,
"2-4-2  Timeout resend                     %s",
    conformance_status (oconf->timeout_resend.test_status)));
  LOG_REPORT ((log_string,
"2-4-3  Busy resend                        %s",
    conformance_status (oconf->busy_resend.test_status)));
  LOG_REPORT ((log_string,
"2-4-4  New on busy                        %s",
    conformance_status (oconf->new_on_busy.test_status)));
  LOG_REPORT ((log_string,
"2-5-1  Multi-byte Data Encoding           %s",
    conformance_status (oconf->multibyte_data_encoding.test_status)));
  LOG_REPORT ((log_string,
"2-6-1  Packet Size Limits                 %s",
    conformance_status (oconf->packet_size_limits.test_status)));
  LOG_REPORT ((log_string,
"2-6-2  Packet size from PD                %s",
    conformance_status (oconf->packet_size_from_pd.test_status)));
  LOG_REPORT ((log_string,
"2-6-3  Packet size stress CP              %s",
    conformance_status (oconf->packet_size_stress_cp.test_status)));
  LOG_REPORT ((log_string,
"2-7-1  Timing                             %s",
    conformance_status (oconf->timing.test_status)));
  LOG_REPORT ((log_string,
"2-7-2  Max delay                          %s",
    conformance_status (oconf->timing.test_status)));
  LOG_REPORT ((log_string,
"2-7-3  Offline test                       %s",
    conformance_status (oconf->offline_test.test_status)));
  LOG_REPORT ((log_string,
"2-8-1  Message Synchronization            %s",
    conformance_status (oconf->message_synchronization.test_status)));
  LOG_REPORT ((log_string,
"2-9-1  Packet Formats                     %s",
    conformance_status (oconf->packet_format.test_status)));
  LOG_REPORT ((log_string,
"2-10-1 SOM Start of Message               %s",
    conformance_status (oconf->SOM.test_status)));
  LOG_REPORT ((log_string,
"2-10-2 SOM sent                           %s",
    conformance_status (oconf->SOM_sent.test_status)));
  LOG_REPORT ((log_string,
"2-11-1 ADDR                               %s",
    conformance_status (oconf->ADDR.test_status)));
  LOG_REPORT ((log_string,
"2-11-2 Address 2                          %s",
    conformance_status (oconf->address_2.test_status)));
  LOG_REPORT ((log_string,
"2-11-3 7F Comset                          %s",
    conformance_status (oconf->address_3.test_status)));
  LOG_REPORT ((log_string,
"2-12-1 LEN                                %s",
    conformance_status (oconf->LEN.test_status)));
  LOG_REPORT ((log_string,
"2-13-1 CTRL                               %s",
    conformance_status (oconf->CTRL.test_status)));
  LOG_REPORT ((log_string,
"2-13-2 Secure Control Block [5]           %s",
    conformance_status (oconf->control_2.test_status)));
  LOG_REPORT ((log_string,
"2-13-3 SCB absent                         %s",
    conformance_status (oconf->scb_absent.test_status)));
  LOG_REPORT ((log_string,
"2-13-4 Sequence numbers                   %s",
    conformance_status (oconf->ctl_seq.test_status)));
  LOG_REPORT ((log_string,
"2-14-1 Security Block (hdr process only)  %s",
    conformance_status (oconf->security_block.test_status)));
  LOG_REPORT ((log_string,
"2-15-1 Incoming C/R valid                 %s",
    conformance_status (oconf->CMND_REPLY.test_status)));
  LOG_REPORT ((log_string,
"2-15-2 No invalid C/R received            %s",
    conformance_status (oconf->invalid_command.test_status)));
  LOG_REPORT ((log_string,
"2-16-1 CHKSUM/CRC16                       %s",
    conformance_status (oconf->CHKSUM_CRC16.test_status)));
  LOG_REPORT ((log_string,
"2-16-2 Checksum                           %s",
    conformance_status (oconf->checksum.test_status)));
  LOG_REPORT ((log_string,
"2-17-1 Large Data Messages                %s",
    conformance_status (oconf->multipart.test_status)));

  LOG_REPORT ((log_string,
"3-1-1  Poll                               %s",
    conformance_status (oconf->cmd_poll.test_status)));
  LOG_REPORT ((log_string,
"3-1-2  Poll raw                           %s",
    conformance_status (oconf->cmd_poll_raw.test_status)));
  LOG_REPORT ((log_string,
"3-1-3  Poll response 3                    %s",
    conformance_status (oconf->cmd_poll_raw.test_status)));
  LOG_REPORT ((log_string,
"3-1-4  Poll response 4                    %s",
    conformance_status (oconf->cmd_poll_raw.test_status)));
  LOG_REPORT ((log_string,
"3-2-1  ID Report Request                  %s",
    conformance_status (oconf->cmd_id.test_status)));
  LOG_REPORT ((log_string,
"3-3-1  Peripheral Device Capabilities Req %s",
    conformance_status (oconf->cmd_pdcap.test_status)));
  LOG_REPORT ((log_string,
"3-4-1  Diagnostic Function Request        %s",
    conformance_status (oconf->cmd_diag.test_status)));
  LOG_REPORT ((log_string,
"3-5-1  Local Status Report Request        %s",
    conformance_status (oconf->cmd_lstat.test_status)));
  LOG_REPORT ((log_string,
"3-6-1  Input Status Report Request        %s",
    conformance_status (oconf->cmd_istat.test_status)));
  LOG_REPORT ((log_string,
"3-7-1  Output Status Report Request       %s",
    conformance_status (oconf->cmd_ostat.test_status)));
  LOG_REPORT ((log_string,
"3-7-2  Ostat ack                          %s",
    conformance_status (oconf->cmd_ostat_ack.test_status)));
  LOG_REPORT ((log_string,
"3-8-1  Reader Status Report Request       %s",
    conformance_status (oconf->cmd_rstat.test_status)));
  LOG_REPORT ((log_string,
"3-9-1  Output Control Command             %s",
    conformance_status (oconf->cmd_out.test_status)));
  LOG_REPORT ((log_string, "3.10 Reader LED Control Command         %s",
    conformance_status (oconf->cmd_led.test_status)));
  LOG_REPORT ((log_string, "3.11 Reader Buzzer Control Command      %s",
    conformance_status (oconf->cmd_buz.test_status)));
  LOG_REPORT ((log_string, "3.12 Reader Text Output Command         %s",
    conformance_status (oconf->cmd_text.test_status)));
  LOG_REPORT ((log_string, "3.14 Communication Configuration Cmd    %s",
    conformance_status (oconf->cmd_comset.test_status)));
  LOG_REPORT ((log_string, "3.16 Set Automatic Rdr Prompt Strings   %s",
    conformance_status (oconf->cmd_prompt.test_status)));
  LOG_REPORT ((log_string, "3.17 Scan and Send Biometric Template   %s",
    conformance_status (oconf->cmd_bioread.test_status)));
  LOG_REPORT ((log_string, "3.18 Scan and Match Biometric Template  %s",
    conformance_status (oconf->cmd_biomatch.test_status)));
  LOG_REPORT ((log_string, "3.20 Manufacturer Specific Command      %s",
    conformance_status (oconf->cmd_mfg.test_status)));
  LOG_REPORT ((log_string, "3.21 Stop Multi Part Message            %s",
"???"));//    conformance_status (oconf->cmd_led.test_status));
  LOG_REPORT ((log_string, "3.22 Maximum Acceptable Reply Size      %s",
"???"));//    conformance_status (oconf->cmd_led.test_status));

  LOG_REPORT ((log_string,
"4-1-1  General Ack Nothing to Report      %s",
    conformance_status (oconf->rep_ack.test_status)));
  LOG_REPORT ((log_string,
"4-2-1  Negative Ack Error Response        %s",
    conformance_status (oconf->rep_nak.test_status)));
  LOG_REPORT ((log_string,
"4-3-1  Device Identification Report       %s",
    conformance_status (oconf->rep_device_ident.test_status)));
  LOG_REPORT ((log_string,
"4-3-2  Ident report consistent            %s",
    conformance_status (oconf->rep_ident_consistent.test_status)));
  LOG_REPORT ((log_string,
"4-4-1  Device Capabilities Report         %s",
    conformance_status (oconf->rep_device_capas.test_status)));
  LOG_REPORT ((log_string,
"4-4-2  Capabilities report consistent     %s",
    conformance_status (oconf->rep_capas_consistent.test_status)));
  LOG_REPORT ((log_string,
"4-5-1 osdp_LSTATR Local Status Report     %s",
    conformance_status (oconf->resp_lstatr.test_status)));
  LOG_REPORT ((log_string,
"4-5-2 osdp_LSTATR Tamper                  %s",
    conformance_status (oconf->resp_lstatr_tamper.test_status)));
  LOG_REPORT ((log_string,
"4-5-3 osdp_LSTATR Power                   %s",
    conformance_status (oconf->resp_lstatr_power.test_status)));
  LOG_REPORT ((log_string,
"4-6-1 osdp_ISTATR Input Status Report     %s",
    conformance_status (oconf->rep_input_stat.test_status)));
  LOG_REPORT ((log_string,
"4-6-2 Input report consistent             %s",
    conformance_status (oconf->rep_input_stat.test_status)));
  LOG_REPORT ((log_string,
"4-7-1 osdp_OSTATR                         %s",
    conformance_status (oconf->rep_output_stat.test_status)));
  LOG_REPORT ((log_string,
"4-7-2 osdp_OSTATR for POLL                %s",
    conformance_status (oconf->resp_ostatr_poll.test_status)));
  LOG_REPORT ((log_string,
"4-7-3 osdp_OSTATR for POLL                %s",
    conformance_status (oconf->resp_ostatr_range.test_status)));
  LOG_REPORT ((log_string,
"4-8-1 osdp_RSTATR                         %s",
    conformance_status (oconf->resp_rstatr.test_status)));

  LOG_REPORT ((log_string,
"=== Passed:   %d",
    oconf->pass));
  LOG_REPORT ((log_string,
"    Failed:   %d",
    oconf->fail));
  LOG_REPORT ((log_string,
"    Untested: %d",
    oconf->untested));
  LOG_REPORT ((log_string,
"    Skipped:  %d",
    oconf->skipped));
  LOG_REPORT ((log_string,
"    Total:    %d",
    oconf->pass + oconf->fail + oconf->untested + oconf->skipped));
  LOG_REPORT ((log_string,
    "---end of report---"));
fprintf (ctx->log, "mmt %d of %d\n",
  oconf->conforming_messages,
  PARAM_MMT);
  if (ctx->report != NULL)
    fclose (ctx->report);

} /* dump_conformance */



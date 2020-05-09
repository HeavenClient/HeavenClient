//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#include "UITermsOfService.h"
#include "UILoginWait.h"

#include "../UI.h"

#include "IO/Components/MapleButton.h"

#include "Net/Packets/LoginPackets.h"

#ifdef USE_NX
#include <nlnx/nx.hpp>
#endif

namespace ms
{
	UITermsOfService::UITermsOfService(std::function<void()> oh) : okhandler(oh), offset(0), unit_rows(1)
	{
		nl::node Login = nl::nx::ui["Login.img"];
		nl::node TOS = Login["TOS"];

		sprites.emplace_back(TOS, Point<int16_t>(399, 250));

		buttons[Buttons::OK] = std::make_unique<MapleButton>(Login["BtOk"], Point<int16_t>(483, 425));
		buttons[Buttons::CANCEL] = std::make_unique<MapleButton>(Login["BtCancel"], Point<int16_t>(557, 425));

		std::string linebreak = "\\r\\n";

#pragma region EULA
		std::string EULA = "NEXON AMERICA, INC. - TERMS OF USE";

		EULA += linebreak;

		EULA += "PLEASE READ THIS TERMS OF USE AGREEMENT CAREFULLY ";
		EULA += "BEFORE USING THE SERVICE.";

		EULA += linebreak;

		EULA += "Nexon America, Inc. (\"Nexon America\" the \"Company.\" \"we.\" \"us";
		EULA += "\") provides this web site (the \"Site\") and all Site-related services and ";
		EULA += "products, including, without limitation, massively multiplayer online ";
		EULA += "role-playing and multiplayer online casual games (collectively, the \"";
		EULA += "Service\") subject to your compliance with the terms and conditions ";
		EULA += "set forth in this agreement (the \"Agreement\"). This Agreement ";
		EULA += "governs the relationship between the Company and you, the Site ";
		EULA += "visitor and/or Service member (\"you\") with respect to your use of the ";
		EULA += "Service. It is important that you read carefully and understand the ";
		EULA += "terms and conditions of this Agreement.";

		EULA += linebreak;

		EULA += "We reserve the right at any time to:";

		EULA += linebreak;

		EULA += "- Change the terms and conditions of this Agreement;";

		EULA += linebreak;

		EULA += "- Change the Service, including terminating, eliminating, ";
		EULA += "supplementing, modifying, adding to or discontinuing any content or ";
		EULA += "data on or feature of the Service or the hours that the Service is ";
		EULA += "available;";

		EULA += linebreak;

		EULA += "- Change any fees or charges, if any, related to your use of the ";
		EULA += "Service; and";

		EULA += linebreak;

		EULA += "- Change the equipment, hardware or software required to use and ";
		EULA += "access the Service.";

		EULA += linebreak;

		EULA += "Any changes we make will be effective immediately upon notice, ";
		EULA += "which we may provide by any means including, without limitation, ";
		EULA += "posting on the Site. Your continued use of the Site or Service after ";
		EULA += "such notice will be deemed acceptance of such changes. Be sure to ";
		EULA += "return to this page periodically to ensure familiarity with the most ";
		EULA += "current version of this Agreement. Upon our request, you agree to sign ";
		EULA += "a non-electronic version of this Agreement.";

		EULA += linebreak;

		EULA += "1. SERVICE REGISTRATION.";

		EULA += linebreak;

		EULA += "1.1 Acceptance of Agreement/Age Requirements. In order to access ";
		EULA += "and use the Service for any purpose, you must sign up for an account (\"";
		EULA += "Account\") with us. Prior to completing the signup process for your ";
		EULA += "Account, you will be required to indicate your acceptance of all of the ";
		EULA += "terms and conditions of this Agreement. If you do not agree to all of ";
		EULA += "the terms and conditions of this Agreement, you may not sign up for an ";
		EULA += "Account and you shall not have the right to use the Service, which ";
		EULA += "includes the right to play any games. By signing up for an Account ";
		EULA += "and using the Service, you represent and warrant that you are 18 years ";
		EULA += "of age or over and have the right, authority and capacity to enter into ";
		EULA += "this Agreement, or you are the legal age required to form a binding ";
		EULA += "contract in your jurisdiction if that age is greater than 18. Without ";
		EULA += "limiting your consent to or the scope of this Agreement or the licenses ";
		EULA += "granted herein, or any future grant of rights, consent, agreements, ";
		EULA += "assignments, and waivers you make with respect thereto, you hereby ";
		EULA += "ratify any prior grant of rights, consents, agreements, assignments and ";
		EULA += "waivers made by you. Your Account may be used only by you, except ";
		EULA += "that if you are a parent or guardian, you may permit one (1) of your ";
		EULA += "minor children who is 13 years of age or older to use the Account ";
		EULA += "instead of you. You agree that you are entirely liable for all activities ";
		EULA += "conducted through the Account, and are responsible for ensuring that ";
		EULA += "you are and/or your child is aware of, understands, and complies with ";
		EULA += "the terms of this Agreement and any and all other Company rules, ";
		EULA += "policies, notices and/or agreements. Notwithstanding the foregoing, ";
		EULA += "you shall at all times be responsible and liable for all activities ";
		EULA += "conducted and items obtained through the Account, including, ";
		EULA += "without limitation, all activities which may be conducted and/or items ";
		EULA += "which may be obtained by your child. Only \"natural persons,\" as ";
		EULA += "opposed to any kinds of legal entities (e.g., corporations, limited ";
		EULA += "liability companies, and/or partnerships), shall have the right to ";
		EULA += "establish a Guest ID Account. We hereby disclaim any and all ";
		EULA += "responsibility and liability for any unauthorized use of your Account.";

		EULA += linebreak;

		EULA += "1.2 Registration Process/Information. To register an Account, you will ";
		EULA += "be required to provide us with certain information about yourself, ";
		EULA += "including, without limitation, your name, e-mail address, birthday, ";
		EULA += "gender, country of residence, state, zip code and ethnicity. We will ";
		EULA += "also ask you to pick a security question and answer (or security ";
		EULA += "questions and answers) and may collect your IP address and ";
		EULA += "computer's hardware and operating system specifications. The ";
		EULA += "information you provide to us and that we collect will be used by us ";
		EULA += "for a variety of internal purposes, including without limitation to verify ";
		EULA += "your rights to and maintain the Account, to ensure that your Account is ";
		EULA += "unique, to deal with security, debugging and technical support issues, ";
		EULA += "for billing and payment-related issues and to protect ourselves and ";
		EULA += "others from abuse. Once we receive your information, we will send ";
		EULA += "you a link to verify your information. You have three (3) days upon ";
		EULA += "receipt of the link to verify your information. Those accounts that are ";
		EULA += "not verified after three (3) days shall be deleted from our database, at ";
		EULA += "which point if you decide you want to maintain an Account with Nexon, ";
		EULA += "you must start the process over again. All of the information you ";
		EULA += "provide to us or that we collect from you will be governed by the terms ";
		EULA += "and conditions of this Agreement and our Privacy Policy. which is ";
		EULA += "hereby incorporated by reference. You agree at all times to (i) provide ";
		EULA += "accurate, current and complete information about yourself as ";
		EULA += "prompted by our registration form or otherwise; and (ii) maintain and ";
		EULA += "update your information (including your e-mail address) to keep it ";
		EULA += "accurate, current and complete. You acknowledge that, if any ";
		EULA += "information provided by you is untrue, inaccurate, not current or ";
		EULA += "incomplete, we reserve the right to terminate this Agreement and your ";
		EULA += "use of the Service and, in our sole discretion, to report you to the ";
		EULA += "appropriate government authorities. We are not responsible or liable ";
		EULA += "if your Account is \"hacked\" or if your Account (or the information ";
		EULA += "contained therein) is otherwise deleted.";

		EULA += linebreak;

		EULA += "1.3 Username/Password. As part of the registration process, you will ";
		EULA += "also be asked to select a username and password for your Account ";
		EULA += "which you will use each time you access the Service. We reserve all ";
		EULA += "right to refuse to grant you or to terminate a username that: (i) ";
		EULA += "impersonates someone else, including, without limitation, another ";
		EULA += "user, Non Playing Character or Game Master (\"GM\"), (ii) is or may be ";
		EULA += "illegal or is or may be protected by trademark or other proprietary ";
		EULA += "rights laws, or may cause confusion, (iii) is or may be considered ";
		EULA += "vulgar, defamatory, obscene, hateful, racially, ethnically or otherwise ";
		EULA += "offensive, including any usernames which are sexual in nature, (iv) is ";
		EULA += "comprised of or includes the name of a popular culture icon, persona ";
		EULA += "or media personality (e.g., \"SpiderMan\" or \"TigerWoods\") or religious ";
		EULA += "deity or figure or your real name or surname, (v) is comprised of or ";
		EULA += "includes the names (including \"street\" names) of any drug, narcotic or ";
		EULA += "other criminal activity, (vi) includes or is comprised of partial or ";
		EULA += "complete sentences (e.g., \"Youwillnotsurvive\"), (vii) is comprised of or ";
		EULA += "includes gibberish (e.g., \"Akdnvprq\"), (viii) is comprised of or includes ";
		EULA += "\"Leet\" or \"Dudespeak\" (e.g., OMGnewb, xLOLx, AFKbotman), (ix) is ";
		EULA += "comprised of or includes any rank and/or fantasy titles (e.g., ";
		EULA += "\"MajorMark\" or \"QueenQiana\"), (x) includes any special characters (i.e.";
		EULA += ", ASCII codes) and/or (xi) is otherwise inappropriate, regardless of our ";
		EULA += "software's ability to disallow such usernames. You agree that you will ";
		EULA += "not use misspellings or alternative spellings or take any other actions ";
		EULA += "for the purpose of circumventing the foregoing restrictions. You ";
		EULA += "understand and agree that, in addition to the foregoing restrictions, we ";
		EULA += "reserve the right to change, remove, alter or delete any username at ";
		EULA += "any time and for any reason in our sole discretion. You will be ";
		EULA += "responsible for the confidentiality and use of your username and ";
		EULA += "password and agree not to transfer your right to use or access the ";
		EULA += "Service via your username or password to any third person (except if ";
		EULA += "you are a parent or guardian to one (1) of your minor children). If you ";
		EULA += "have reason to believe that your Account with us is no longer secure, ";
		EULA += "you must promptly change your password by visiting http://www.nexon.";
		EULA += "net and immediately notify us of the problem by e-mailing our ";
		EULA += "customer service department by using the E-mail of Inquiry form on the ";
		EULA += "Site. YOU ARE ENTIRELY RESPONSIBLE FOR MAINTAINING THE ";
		EULA += "CONFIDENTIALITY OF YOUR USERNAME AND PASSWORD AND ";
		EULA += "FOR ANY AND ALL ACTIVITIES (INCLUDING PURCHASES AND ";
		EULA += "CHARGES, AS APPLICABLE) THAT ARE CONDUCTED THROUGH ";
		EULA += "YOUR ACCOUNT.";

		EULA += linebreak;

		EULA += "2. SERVICE USE & RESTRICTIONS.";

		EULA += linebreak;

		EULA += "2.1 Basic Use. You may use the Service solely subject to the terms ";
		EULA += "and conditions set forth in this Agreement. You may use the Software ";
		EULA += "(as defined in the End User License Agreement) to access the ";
		EULA += "Service solely subject to the terms and conditions of the End User ";
		EULA += "License Agreement as well as this Agreement. You understand ";
		EULA += "and agree, however, that you are solely responsible for obtaining and ";
		EULA += "maintaining all telephone, cable, wireless, computer hardware and ";
		EULA += "other equipment needed to access and use the Service and that you ";
		EULA += "shall be solely responsible for all charges and fees related thereto.";

		EULA += linebreak;

		EULA += "2.2 Code of Conduct. While using the Service and Software, you ";
		EULA += "agree to comply with all applicable laws, rules and regulations. We ";
		EULA += "reserve the right, in our sole discretion, to take any actions we deem ";
		EULA += "necessary and appropriate to preserve the integrity of the Service and ";
		EULA += "Software. We further reserve the right to determine what conduct is ";
		EULA += "outside the \"spirit\" of the Service and games provided therein. While ";
		EULA += "using the Service, you agree not to:";

		EULA += linebreak;

		EULA += "- Restrict or inhibit any other individual from using the Service, a ";
		EULA += "Cash Item (as defined below) or the Software, including, without ";
		EULA += "limitation, by means of \"hacking\" or defacing any portion of the ";
		EULA += "Service or Software;";

		EULA += linebreak;

		EULA += "- Institute an attack upon any server used in connection with the ";
		EULA += "Service or otherwise attempt to disrupt such servers;";

		EULA += linebreak;

		EULA += "- Use the Service, Cash Items or Software for any unlawful purpose or ";
		EULA += "in any manner not intended by the Company as contemplated herein ";
		EULA += "and/or on the Site;";

		EULA += linebreak;

		EULA += "- Engage in rude, unlawful, harassing, vulgar, obscene, hateful, ";
		EULA += "threatening, abusive or otherwise objectionable behavior, including, ";
		EULA += "without limitation, looting, kill stealing, making sexual comments ";
		EULA += "and/or cursing;";

		EULA += linebreak;

		EULA += "- Express or imply that any statements you make or actions you take ";
		EULA += "are endorsed by us;";

		EULA += linebreak;

		EULA += "- Impersonate another person (including celebrities), indicate that you ";
		EULA += "are a Nexon America employee, representative of Nexon America ";
		EULA += "(including a GM) or attempt to mislead users by indicating that you ";
		EULA += "represent Nexon America or any of Nexon America's partners or ";
		EULA += "affiliates;";

		EULA += linebreak;

		EULA += "- Transmit: (i) any content that is unlawful, harassing, vulgar, obscene, ";
		EULA += "hateful, fraudulent, threatening, abusive, libelous, defamatory, ";
		EULA += "obscene, sexually explicit, or racially, ethnically or otherwise ";
		EULA += "objectionable, or that could be deemed to be stalking or constitute an ";
		EULA += "invasion of a right of privacy of another person; (ii) any content that ";
		EULA += "infringes our or any third party's intellectual property or other rights, or ";
		EULA += "that you otherwise do not have permission to transmit; (iii) any software ";
		EULA += "or other materials that contain any viruses, worms, trojan horses, ";
		EULA += "defects, date bombs, time bombs or other items of a destructive ";
		EULA += "nature; (iv) any material, non-public information about companies ";
		EULA += "without the authorization to do so; (v) any trade secret of any third ";
		EULA += "party; or (vi) any advertisements, solicitations, chain letters, pyramid ";
		EULA += "schemes, investment opportunities or other unsolicited commercial ";
		EULA += "communication (except as otherwise expressly permitted by us);";

		EULA += linebreak;

		EULA += "- Engage in spamming or flooding;";

		EULA += linebreak;

		EULA += "- Remove, alter or conceal any copyright, trademark, patent or other ";
		EULA += "proprietary rights notices contained in the Service, Cash Items or ";
		EULA += "Software;";

		EULA += linebreak;

		EULA += "- \"Frame\" or \"mirror\" any part of the Service without our prior written ";
		EULA += "authorization;";

		EULA += linebreak;

		EULA += "- Link to any page of or content on the Site other than the URL ";
		EULA += "located at http://www.nexon.net/;";

		EULA += linebreak;

		EULA += "- Provide a link to any web sites that promote any product or any ";
		EULA += "service;";

		EULA += linebreak;

		EULA += "- Use any robot, spider, site search/retrieval application or other ";
		EULA += "manual or automatic device or process to retrieve, index, \"data mine\" ";
		EULA += "or in any way reproduce or circumvent the navigational structure or ";
		EULA += "presentation of the Service or its contents, including, but not limited to, ";
		EULA += "Cash Items;";

		EULA += linebreak;

		EULA += "- Harvest or collect information about the Service, visitors to the Site ";
		EULA += "or users of the Service without their express consent;";

		EULA += linebreak;

		EULA += "- Download, copy, reproduce, republish, upload, post, transmit, ";
		EULA += "modify, distribute or publicly display any of the content or information ";
		EULA += "contained in the Service except as expressly authorized by this ";
		EULA += "Agreement;";

		EULA += linebreak;

		EULA += "- Host, provide matchmaking services for, or emulate or redirect the ";
		EULA += "communications protocols used by us (or our designees) as part of the ";
		EULA += "Service, including, without limitation, by protocol emulation, ";
		EULA += "tunneling, reverse engineering, modifying the Software or using a ";
		EULA += "utility program to host the Software;";

		EULA += linebreak;

		EULA += "- Sell, advertise, or post information on hacks for the Software, Cash ";
		EULA += "Items or Service and/or advertising, posting information on or selling ";
		EULA += "hacks for any other software or web sites;";

		EULA += linebreak;

		EULA += "- Exploit the Software, Cash Items or the Service for any commercial ";
		EULA += "purpose, including the provision of \"power leveling\" services;";

		EULA += linebreak;

		EULA += "- Exploit errors in design, features which are not documented and/or ";
		EULA += "bugs to gain access that would otherwise not be available or to obtain ";
		EULA += "any competitive advantage;";

		EULA += linebreak;

		EULA += "- Modify the Software, Cash Items or the Service to change \"game ";
		EULA += "play,\" including, without limitation, creating cheats and/or hacks or ";
		EULA += "using third-party software to access files in the Software or Service;";

		EULA += linebreak;

		EULA += "- Reverse engineer, decompile or disassemble all or any portion of ";
		EULA += "the Service, Cash Items, or Software (code included);";

		EULA += linebreak;

		EULA += "- Use tools which hack or alter the Software, Cash Items or the Service ";
		EULA += "or that allow you to connect to the Software's private binary interface ";
		EULA += "or utilize user or other interfaces other than those provided by us to ";
		EULA += "you;";

		EULA += linebreak;

		EULA += "- Engage in using 3rd party macros (i.e., \"macroing\"), unauthorized ";
		EULA += "auto-looting or robot play (i.e., \"botting\") or any other behavior that ";
		EULA += "allows you (or any character you are controlling) to automatically ";
		EULA += "function or effect any action in a game with or - Use \"packet sniffing,\" ";
		EULA += "scripting and/or macroing software for any purpose whatsoever, or ";
		EULA += "otherwise monitor the Service, the Cash Items or the Software;";

		EULA += linebreak;

		EULA += "- Attempt to obtain a password or other private account information ";
		EULA += "from any other person or user of the Service; and/or";

		EULA += linebreak;

		EULA += "- Disclose your personally identifiable information or any other ";
		EULA += "person's or user's personally identifiable information (e.g., name, ";
		EULA += "e-mail address, telephone number, age, address, etc.) on the Site or ";
		EULA += "through the Service, or web sites or forums related to the Site or ";
		EULA += "Service, including without limitation the Forums discussed in Section 2. ";
		EULA += "3.";

		EULA += linebreak;

		EULA += "2.3 Playing Games. At all times you shall comply with the terms and ";
		EULA += "conditions of this Agreement and the terms and conditions of all Game ";
		EULA += "Rules and Policies provided on the Service, which are hereby ";
		EULA += "incorporated herein by this reference. By playing any of the games ";
		EULA += "provided on the Service, you agree to the Game Rules and Policies";
		EULA += "applicable to each game, respectively.";

		EULA += linebreak;

		EULA += "2.3.1 Game Forums. While playing games, you may create and/or ";
		EULA += "join existing forums as part of the Service (the \"Forums\") for the ";
		EULA += "purpose of communicating with other players regarding game strategy. ";
		EULA += "During the registration process, you will be required to select a Forum ";
		EULA += "name, such name to be subject to the restrictions applicable to ";
		EULA += "usernames and group names as set forth in Section 1.3 above. While ";
		EULA += "participating in a Forum, in addition to adhering to the rules of ";
		EULA += "conduct set forth in Section 2.2 above, you agree not to:";

		EULA += linebreak;

		EULA += "- Disrupt the normal course and pace of postings or chat in the Forum, ";
		EULA += "including through: (i) use of a macro with large amounts of text; (ii) ";
		EULA += "use of mechanisms causing the Forum or chat screen to scroll faster ";
		EULA += "than other users are able to read; (iii) hitting the return key repeatedly; ";
		EULA += "(iv) excessive shouting through the use of the \"all caps\" key; (v) posting ";
		EULA += "\"Spam\" messages; and/or (vi) sending repeated unsolicited or ";
		EULA += "unwelcome messages to a single user or repeatedly posting similar ";
		EULA += "messages in a Forum or chat area;";

		EULA += linebreak;

		EULA += "- Harass, threaten, stalk, embarrass or cause distress, unwanted ";
		EULA += "attention or discomfort to any other person or player; and";

		EULA += linebreak;

		EULA += "- Participate in any action that, in the sole discretion of the Company, ";
		EULA += "\"scams\" or otherwise defrauds any other player, including with respect ";
		EULA += "to any items that a player has earned or otherwise obtained (e.g., Cash ";
		EULA += "Items).";

		EULA += linebreak;

		EULA += "You understand that much of the information included in the Forums ";
		EULA += "is from other players who are not employed by or under the control of ";
		EULA += "the Company. You further acknowledge that a large volume of ";
		EULA += "information is available in the Forums and that people participating in ";
		EULA += "such Forums occasionally post messages or make statements, whether ";
		EULA += "intentionally or unintentionally, that are inaccurate, misleading, ";
		EULA += "deceptive, abusive or even unlawful. We neither endorse nor are ";
		EULA += "responsible for such messages or statements, or for any opinion, advice, ";
		EULA += "information or other utterance made or displayed in the Forums by you ";
		EULA += "or the other players. The opinions expressed in the Forums reflect ";
		EULA += "solely the opinion(s) of you and the other players and may not reflect ";
		EULA += "the opinion(s) of the Company. We are not responsible for any errors ";
		EULA += "or omissions in postings, for hyperlinks embedded in messages or for ";
		EULA += "any results obtained from the use of the information contained in the";
		EULA += "Forums. Under no circumstances will we or our affiliates, suppliers or ";
		EULA += "agents be liable for any loss or damage caused by your reliance on the ";
		EULA += "information in the Forums or your use of the Forums. You should be ";
		EULA += "aware that, when you disclose information about yourself in a Forum ";
		EULA += "while using the Service, the information is being made publicly ";
		EULA += "available and may be collected and used by other users. When you ";
		EULA += "disclose any information in a Forum, you do so at your own risk. We ";
		EULA += "have no obligation to monitor the Forums, or any postings or other ";
		EULA += "materials that you or other players transmit or post on the Forums. ";
		EULA += "However, you acknowledge and agree that we have the right (but not ";
		EULA += "the obligation) to monitor the Forums and the postings and other ";
		EULA += "materials you and the other players transmit and post; to alter or ";
		EULA += "remove any such materials (including, without limitation, any posting ";
		EULA += "to a Forum); and to disclose such materials and the circumstances ";
		EULA += "surrounding their transmission to any third party in order to operate the ";
		EULA += "Service properly, to protect ourselves, our sponsors and our members ";
		EULA += "and visitors and/or to comply with legal obligations or governmental ";
		EULA += "requests.";

		EULA += linebreak;

		EULA += "2.3.2 Cash Shop / Cash Items. While playing our games, you will have ";
		EULA += "the opportunity to visit our online store (\"Cash Shop\") and use online ";
		EULA += "\"cash\" (\"NX\") to license a variety of virtual items (\"Cash Items\"), in ";
		EULA += "addition to purchasing 'character transfers' and 'character name ";
		EULA += "changes' that can be used while playing various games. In order to ";
		EULA += "obtain NX, you may be required to provide us or another payment ";
		EULA += "service designated by us (e.g., PayPal) with your credit card ";
		EULA += "information and other information related to your credit card ";
		EULA += "transaction (e.g., your billing and shipping address on record with the ";
		EULA += "applicable credit card company, your credit card expiration date, etc.). ";
		EULA += "For each \"real world\" U.S. Dollar that you authorize us to charge to ";
		EULA += "your credit card, you will be awarded one thousand (1000) units of NX; ";
		EULA += "provided, however, that we reserve the right in our sole discretion at ";
		EULA += "any time to change the number of units you will be entitled to for each ";
		EULA += "\"real world\" U.S. Dollar. YOU UNDERSTAND AND AGREE THAT ";
		EULA += "ONCE YOU AUTHORIZE US OR OTHER DESIGNATED PAYMENT ";
		EULA += "SERVICE TO CHARGE YOUR CREDIT CARD FOR A CERTAIN ";
		EULA += "AMOUNT, SUCH AMOUNT SHALL UNDER NO CIRCUMSTANCES BE ";
		EULA += "REFUNDABLE, INCLUDING, WITHOUT LIMITATION, UPON ";
		EULA += "TERMINATION OF YOUR ACCOUNT FOR ANY REASON, ";
		EULA += "TERMINATION OF THIS AGREEMENT, TERMINATION OF THE ";
		EULA += "CASH SHOP AND/OR THE DISCONTINUATION OF THE SERVICE. ";
		EULA += "You may also be awarded points (\"Game Points\") for use in the Cash ";
		EULA += "Shop or through certain of our promoted events. These Game Points ";
		EULA += "have no \"real world\" value, but may be exchanged by you for ";
		EULA += "designated Cash Items in the Cash Shop. Certain Cash Items have an ";
		EULA += "expiration date, while others have no expiration date (\"Lifetime ";
		EULA += "Items\"). Each Cash Item that you obtain using NX or Game Points will ";
		EULA += "be included in your Account until the earlier of that Cash Item's ";
		EULA += "expiration date (if it is not a Lifetime Item), your Account's expiration ";
		EULA += "or termination date, or such date when the Service ends. ";
		EULA += "REGARDLESS OF THE CONSIDERATION OFFERED OR PAID IN ";
		EULA += "EXCHANGE FOR CASH ITEMS, YOU 00 NOT HAVE ANY ";
		EULA += "OWNERSHIP RIGHTS IN THE CASH ITEMS. We have no liability for ";
		EULA += "\"hacking\" or loss of your Cash Items from your Account, provided we ";
		EULA += "will use reasonable efforts to replace such items under certain ";
		EULA += "circumstances in our reasonable discretion. We have no obligation or ";
		EULA += "responsibility to and will not reimburse you for any Cash Item or any ";
		EULA += "experience lost due to your violations of this and any other Company ";
		EULA += "rules, policies, notices and/or agreements. You understand that any ";
		EULA += "credit card transaction-related information will be treated by the ";
		EULA += "Company in the manner described herein and in our Privacy Policy, ";
		EULA += "and, as applicable, in the manner described in the privacy policy of ";
		EULA += "any third-party payment service that we choose to use. You agree that ";
		EULA += "all information that you provide to the Company or a designated ";
		EULA += "third-party payment service will be accurate, current and complete. ";
		EULA += "You hereby agree to pay all charges incurred by you (or your child, if ";
		EULA += "applicable) resulting from your use of the Service at the price(s) in ";
		EULA += "effect when such charges are incurred. You will also be responsible ";
		EULA += "for paying any applicable taxes relating to such transactions. You ";
		EULA += "understand that we may suspend or terminate your Account if for any ";
		EULA += "reason a charge you authorize us to make to your credit card cannot be ";
		EULA += "processed or is returned to us unpaid and, if such event occurs, you ";
		EULA += "shall immediately remit to us payment for such charge through the use ";
		EULA += "of another credit card or other payment mechanism. We shall not be ";
		EULA += "responsible or liable for any credit card or bank-related charges or fees ";
		EULA += "related to any of your transactions. We reserve the right, without prior ";
		EULA += "notification, to limit the order quantity on any Cash Item and/or to ";
		EULA += "refuse to provide you with any Cash Item. Verification of certain ";
		EULA += "information applicable to a transaction involving a Cash Item may be ";
		EULA += "required prior to our acceptance thereof. Price and availability of the ";
		EULA += "Cash Items are subject to change without notice. You agree that you ";
		EULA += "cannot and have no right to sell or otherwise transfer any of the Cash ";
		EULA += "Items, or any other content or information included in the Service, in ";
		EULA += "whole or in part, to any third person or entity whatsoever, including, ";
		EULA += "without limitation, on Internet auction sites (e.g., eBay, IGE) or in ";
		EULA += "return for anything of value (including \"real\" money) or otherwise. ";
		EULA += "With respect to the MapleStory Trading System, you acknowledge that ";
		EULA += "the items transacted therein will not be transferred with your character ";
		EULA += "when you transfer said character to a different world or change the ";
		EULA += "character name. In such cases, in order to avoid any item losses, you ";
		EULA += "must first finalize and/or cancel all your transactions in the MapleStory ";
		EULA += "Trading System and keep your items in your Item Inventory prior to ";
		EULA += "making any requests for 'character transfers' and/or 'character name ";
		EULA += "changes'.";

		EULA += linebreak;

		EULA += "2.4 Hardware and Software Access. You hereby acknowledge that:";

		EULA += linebreak;

		EULA += "2.4.1 The Company has the right to obtain, without notification to you, ";
		EULA += "certain information about your computer or software, including, but ";
		EULA += "not limited to, your operating system, identification of your hard drives, ";
		EULA += "central processing unit, IP address, and Internet browser for purposes ";
		EULA += "of identification.";

		EULA += linebreak;

		EULA += "2.4.2 The Company has the right to obtain, without notification to you, ";
		EULA += "non-personal information from your connection to the Service or Site ";
		EULA += "for demographic purposes.";

		EULA += linebreak;

		EULA += "2.4.3 The Company has the right to obtain, without notification to you, ";
		EULA += "information from your computer, software, and parts or portions thereof, ";
		EULA += "including, without limitation, your computer's random access memory, ";
		EULA += "video card, central processing unit, hard drive(s) and any other storage ";
		EULA += "devices to assist our efforts in policing users who may develop and/or ";
		EULA += "use \"hacks\" and/or \"cheats\" to gain advantage over other users. The ";
		EULA += "information obtained in this Section will only be used for the purpose ";
		EULA += "of identifying persons or entities not in compliance or believed by the ";
		EULA += "Company to not be in compliance with this Agreement and any and ";
		EULA += "all other Company rules, policies, notices and/or agreements.";

		EULA += linebreak;

		EULA += "3. COMPANY MATERIALS. The Software and Service are intended ";
		EULA += "solely for playing purposes and for your personal use. You may print a ";
		EULA += "single copy of any textual material available for downloading through ";
		EULA += "the Service. Although the Company strives to provide content through ";
		EULA += "its Service that is both useful and accurate, data and other information ";
		EULA += "change frequently and are subject to varying interpretations. ";
		EULA += "Accordingly, although the Company endeavors to use reasonable care ";
		EULA += "in assembling such content, it may not be up-to-date, accurate or ";
		EULA += "complete. In addition, portions of such content may have been ";
		EULA += "contributed by various third parties and/or service providers. The ";
		EULA += "inclusion of such information does not indicate any approval or ";
		EULA += "endorsement of such third parties or providers and the Company ";
		EULA += "expressly disclaims any liability with respect to the foregoing. ";
		EULA += "Descriptions or images of, or references to, products or services ";
		EULA += "available on the Service do not imply the Company's endorsement of ";
		EULA += "such products or services. If you believe that certain content is ";
		EULA += "incomplete or inaccurate, please contact our customer service ";
		EULA += "department using the E-mail of Inquiry located on the Site with, if ";
		EULA += "possible, a description of the content to be checked and the location ";
		EULA += "(URL) where such content may be found.";

		EULA += linebreak;

		EULA += "4. LINKS. The Site may contain links to other Internet web sites, ";
		EULA += "including affiliated web sites, which may or may not be owned or ";
		EULA += "operated by the Company. The Company has not reviewed all of the ";
		EULA += "web sites that are linked to the Site, and the Company has no control ";
		EULA += "over such sites. The Company is not responsible for the content of ";
		EULA += "such web sites, any updates or changes to such sites, or the privacy or ";
		EULA += "other practices of such sites, and the fact that the Company offers such ";
		EULA += "links does not indicate any approval or endorsement of any material ";
		EULA += "contained on any linked site. The Company is providing these links to ";
		EULA += "you only as a convenience. Accordingly, we strongly encourage you to ";
		EULA += "become familiar with the terms of use and practices of any linked site. ";
		EULA += "Further, it is up to you to take precautions to ensure that whatever links ";
		EULA += "you select or software you download from such web sites is free of such ";
		EULA += "items as viruses, worms, trojan horses, defects, date bombs, time ";
		EULA += "bombs and other items of a destructive nature.";

		EULA += linebreak;

		EULA += "5. CLAIMS OF COPYRIGHT INFRINGEMENT. The Digital ";
		EULA += "Millennium Copyright Ad of 1998 (the \"DMCA\") provides recourse for ";
		EULA += "copyright owners who believe that material appearing on the Internet ";
		EULA += "infringes their rights under U.S. copyright law. If you believe in good ";
		EULA += "faith that materials hosted by the Company infringe your copyright (for ";
		EULA += "example, materials posted on one of our Forums), you (or your agent) ";
		EULA += "may send us a notice requesting that the material be removed or ";
		EULA += "access to it blocked. The notice must include the following ";
		EULA += "information: (i) a physical or electronic signature of a person ";
		EULA += "authorized to act on behalf of the owner of an exclusive right that is ";
		EULA += "allegedly infringed; (ii) identification of the copyrighted work claimed ";
		EULA += "to have been infringed (or if multiple copyrighted works located on the ";
		EULA += "Site are covered by a single notification, a representative list of such ";
		EULA += "works); (iii) identification of the material that is claimed to be infringing ";
		EULA += "or the subject of infringing activity, and information reasonably ";
		EULA += "sufficient to allow the Company to locate the material on the Site; (iv) ";
		EULA += "the name, address, telephone number and email address (if ";
		EULA += "available) of the complaining party; (v) a statement that the ";
		EULA += "complaining party has a good faith belief that use of the material in ";
		EULA += "the manner complained of is not authorized by the copyright owner, ";
		EULA += "its agent or the law; and (vi) a statement that the information in the ";
		EULA += "notification is accurate and, under penalty of perjury, that the ";
		EULA += "complaining party is authorized to act on behalf of the owner of an ";
		EULA += "exclusive right that is allegedly infringed. If you believe in good faith ";
		EULA += "that a notice of copyright infringement has been wrongly filed by the ";
		EULA += "Company against you, the DMCA permits you to send the Company a ";
		EULA += "counter-notice. Notices and counter-notices must meet the ";
		EULA += "then-current statutory requirements imposed by the DMCA; see ";
		EULA += "http://www.loc.gov/copyright/ for details. Notices and counter-notices ";
		EULA += "with respect to the Site should be sent to Nexon America's Copyright ";
		EULA += "Agent for Notice of claims of copyright infringement as follows:";

		EULA += linebreak;

		EULA += "Nexon America, Inc.";
		EULA += "137 N. Larchmont Blvd., #619";
		EULA += "Los Angeles, CA 90004-3704";
		EULA += "Attn.: Legal Department";
		EULA += "By email: press@wizet.com";

		EULA += linebreak;

		EULA += "The Nexon America copyright agent should be contacted only if you ";
		EULA += "believe that your work has been used or copied in a way that ";
		EULA += "constitutes copyright infringement and such infringement is occurring ";
		EULA += "through use of the Service or Site. Please note that these ";
		EULA += "notifications and counter-notifications are legal notices. Nexon ";
		EULA += "America may provide copies of such notices to the participants in the ";
		EULA += "dispute or third parties, at our discretion and as required by law. Our ";
		EULA += "Privacy Policy does not protect information provided in these notices ";
		EULA += "and counter-notices. ALL OTHER INQUIRIES DIRECTED TO THE ";
		EULA += "NEXON AMERICA COPYRIGHT AGENT WILL NOT BE ANSWERED.";

		EULA += linebreak;

		EULA += "6. OWNERSHIP, LICENSING AND RESTRICTIONS 0N USE.";

		EULA += linebreak;

		EULA += "6.1 Ownership. Except for the personal information and content ";
		EULA += "submitted by users of the Service, the Service and Software and all ";
		EULA += "content included therein (including without limitation graphics, artwork, ";
		EULA += "music, choreography, characters, character IDs, Forum IDs, account IDs, ";
		EULA += "Cash Items and/or other items acquired or created in the Service, ";
		EULA += "including through the Cash Shop) are owned by Nexon America or its ";
		EULA += "licensors, and are protected by United States and other international ";
		EULA += "intellectual property laws. You may not use our trademarks and trade ";
		EULA += "dress in connection with any product or service that is not ours. in any ";
		EULA += "manner that is likely to cause confusion among customers or ";
		EULA += "subscribers, or in any manner that disparages us. Except as expressly ";
		EULA += "provided herein, we and our licensors do not grant you any express or ";
		EULA += "implied rights, and all rights, title and interest that we have in and to ";
		EULA += "the Service that are not expressly granted by us to you are retained by ";
		EULA += "us.";

		EULA += linebreak;

		EULA += "6.2 Your License from Nexon America. Subject to the terms of this ";
		EULA += "Agreement and so long as you remain compliant with such ";
		EULA += "Agreement, Nexon America grants you a non-exclusive, limited, fully ";
		EULA += "revocable license to use the Service, and the content contained ";
		EULA += "therein in conjunction with the Service. You may not modify, publish, ";
		EULA += "transmit, transfer, sell, reproduce, upload, post, distribute, perform, ";
		EULA += "display, create derivative works from, or in any way exploit such ";
		EULA += "content, except as Nexon America expressly permits in this Agreement ";
		EULA += "or the Service. Your use of such content for any purpose other than as ";
		EULA += "expressly permitted in this Agreement or the Service is a violation of ";
		EULA += "the intellectual property rights and other proprietary rights of Nexon ";
		EULA += "America and may subject you to civil liability and/or criminal ";
		EULA += "prosecution under applicable laws.";

		EULA += linebreak;

		EULA += "6.3 Your License to Nexon America. When you provide content to or ";
		EULA += "create content using the Service, you grant us a non-exclusive, ";
		EULA += "worldwide, perpetual, irrevocable, royalty-free, fully paid-up, ";
		EULA += "sublicensable right throughout the world to exercise all copyright, ";
		EULA += "publicity, and other rights you have in the content in any media known ";
		EULA += "now or in the future. Such rights include, without limitation, all rights ";
		EULA += "you have in use, distribution, reproduction, modification, adaptation, ";
		EULA += "creation of derivative works, translation, public performance and public ";
		EULA += "display of the content. You also hereby waive any moral rights you ";
		EULA += "may have in such content under the laws of any jurisdiction. You ";
		EULA += "hereby appoint us as your agent with full power to enter into and ";
		EULA += "execute any document and/or do any ad we may consider appropriate ";
		EULA += "to confirm the grant of rights, consents, agreements, assignments and ";
		EULA += "waivers set forth in these Terms of Use. We reserve the right to remove ";
		EULA += "or modify any content you provide to us or otherwise post on the Site ";
		EULA += "at our sole discretion and without prior notice or any liability to you. ";
		EULA += "You represent, warrant and agree that none of the content you provide ";
		EULA += "to or create using the Service violates any third party's intellectual ";
		EULA += "property or other rights or is subject to any obligation, whether of ";
		EULA += "confidentiality, attribution or otherwise, on our part and we will not be ";
		EULA += "liable for any use or disclosure thereof.";

		EULA += linebreak;

		EULA += "6.4 Software. Use or downloading of the Software is conditioned on ";
		EULA += "your acceptance of the terms and conditions of our End User License ";
		EULA += "Agreement and any license agreements relating to such Software, ";
		EULA += "including all third party agreements. By using the Software, you agree ";
		EULA += "to all of the terms and conditions set forth in such agreements.";

		EULA += linebreak;

		EULA += "7. ACCESS & USE BY MINORS. Pursuant to 47 U.S.C. Section ";
		EULA += "230(d), as amended, we hereby notify you that parental control ";
		EULA += "protections (such as computer hardware, software or filtering services) ";
		EULA += "are commercially available that may assist you in limiting access to ";
		EULA += "material that is harmful to minors. Information identifying current ";
		EULA += "providers of protections is available from America Links Up: http://www.";
		EULA += "netparents.org/parentstips/browsers.html.";

		EULA += linebreak;

		EULA += "8. PRIVACY/SECURITY. You understand that any information ";
		EULA += "provided by you or collected by us in connection with your use of the ";
		EULA += "Service will be used in the manner described herein and pursuant to ";
		EULA += "the terms and conditions of our Privacy Policy, such Privacy Policy ";
		EULA += "being incorporated into and made a part of this Agreement by this ";
		EULA += "reference. If you do not agree to the terms of the Privacy Policy, you ";
		EULA += "may not use the Service. Without limiting the terms of the Privacy ";
		EULA += "Policy, you understand that we do not guarantee that your use of the ";
		EULA += "Service and/or the information contained in your Account will be ";
		EULA += "private or secure, and we are not responsible or liable to you for any ";
		EULA += "lack of privacy or security you may experience. You are fully ";
		EULA += "responsible for taking precautions and providing security measures ";
		EULA += "best suited for your situation and intended use of the Service.";

		EULA += linebreak;

		EULA += "9. JURISDICTIONAL ISSUES/SERVICE INTERRUPTIONS. The ";
		EULA += "Service is not available or accessible in countries or territories where ";
		EULA += "we are operating a localized version of the Service through a licensed ";
		EULA += "operator, such as China, Japan, Taiwan, Singapore, Malaysia and ";
		EULA += "Thailand. We make no representation that materials available on or ";
		EULA += "through the Service, including, without limitation, the Software and ";
		EULA += "the games, are appropriate or available for use in all locations. Those ";
		EULA += "who choose to access and/or use the Service do so on their own ";
		EULA += "initiative and at their own risk, and are responsible for compliance with ";
		EULA += "local laws, if and to the extent local laws are applicable. The ";
		EULA += "Software is subject to United States export controls as set forth in the ";
		EULA += "End User License Agreement. We reserve the right to limit, suspend, ";
		EULA += "interrupt or terminate the availability of the Service, in whole or in ";
		EULA += "part, to you and any other user or person, geographic area or ";
		EULA += "jurisdiction, at any time and in our sole discretion. You acknowledge ";
		EULA += "and agree that interruptions, delays and disruptions of the Service may ";
		EULA += "occur and that the Company has no control over third party servers, ";
		EULA += "systems and/or networks which may be utilized in connection with the ";
		EULA += "functioning of the Service. The Company hereby disclaims all ";
		EULA += "liabilities with respect to all interruptions, delays and disruptions of the ";
		EULA += "Service.";

		EULA += linebreak;

		EULA += "10. TERMINATION AND DISCONTINUATION";

		EULA += linebreak;

		EULA += "10.1 Termination. This Agreement shall remain effective until ";
		EULA += "terminated. If you wish to terminate your Account, you may do so by ";
		EULA += "contacting our customer service department by using the E-mail of ";
		EULA += "Inquiry on the Site or by terminating your account by following the ";
		EULA += "instructions on the Site. Upon our acceptance of your request, your ";
		EULA += "Account will be terminated. We reserve the right, with or without ";
		EULA += "notice to you, to suspend or terminate your Account and this ";
		EULA += "Agreement if you violate the terms and conditions of: (i) this ";
		EULA += "Agreement, Iii) the End User License Agreement, and/or (iii) any of the ";
		EULA += "Game Rules and Policies. We also reserve the right, with or without ";
		EULA += "notice to you, to suspend or terminate your Account and this ";
		EULA += "Agreement in our reasonable discretion.";

		EULA += linebreak;

		EULA += "10.2 Discontinuation. You understand that the Service is provided via ";
		EULA += "the Internet by the Company through the use of servers, networks and ";
		EULA += "other technology. Notwithstanding anything to the contrary in this ";
		EULA += "Agreement (including without limitation Section 10.1), the Company ";
		EULA += "reserves the right to permanently discontinue the Service at any time. ";
		EULA += "Upon such discontinuation, your Account and this Agreement shall ";
		EULA += "terminate.";

		EULA += linebreak;

		EULA += "10.3 Effect of Termination. Upon termination of this Agreement for ";
		EULA += "any reason, your right to use the Service shall immediately cease, and ";
		EULA += "you shall destroy all Software and other content obtained through ";
		EULA += "your use of the Service and all copies thereof. YOU ACKNOWLEDGE ";
		EULA += "AND AGREE THAT WE SHALL HAVE N0 OBLIGATION ";
		EULA += "WHATSOEVER TO YOU AFTER THE TERMINATION OF YOUR ";
		EULA += "ACCOUNT OR THIS AGREEMENT, INCLUDING, WITHOUT ";
		EULA += "LIMITATION, TO PROVIDE YOU WITH A REFUND OF ANY ";
		EULA += "CHARGES YOU AUTHORIZED US OR ANY DESIGNATED PAYMENT ";
		EULA += "SERVICE TO CHARGE TO YOUR CREDIT CARD(S) IN ";
		EULA += "CONNECTION WITH YOUR PURCHASE OF NX, WHETHER OR NOT ";
		EULA += "YOU ACTUALLY USED THE NX TO OBTAIN CASH ITEMS.";

		EULA += linebreak;

		EULA += "10.4 Survival. Sections 6.1, 6.3, 10.3, 10.4, 11, 12, 13, and 16 (as ";
		EULA += "well as the definitions applicable to such sections) shall survive any ";
		EULA += "termination of this Agreement or discontinuance of the Service.";

		EULA += linebreak;

		EULA += "11. DISCLAIMERS. THE SERVICE (INCLUDING THE SITE, THE ";
		EULA += "GAMES, THE CASH ITEMS, THE SOFTWARE AND ALL OTHER ";
		EULA += "CONTENT CONTAINED THEREIN) AND ANY PRODUCT OR SERVICE ";
		EULA += "OBTAINED THROUGH THE SERVICE ARE PROVIDED ON AN \"AS ";
		EULA += "IS\" AND \"AS AVAILABLE\" BASIS AND WITHOUT WARRANTIES OF ";
		EULA += "ANY KIND, EITHER EXPRESS OR IMPLIED. TO THE FULLEST ";
		EULA += "EXTENT PERMISSIBLE PURSUANT TO APPLICABLE LAW, THE ";
		EULA += "COMPANY AND ITS PARENTS, SUBSIDIARIES, AFFILIATES, ";
		EULA += "LICENSORS, SUPPLIERS, ADVERTISERS, PARTNERS, SPONSORS ";
		EULA += "AND AGENTS DISCLAIM ALL WARRANTIES, EXPRESS OR IMPLIED, ";
		EULA += "WHICH MIGHT APPLY TO THE SERVICE, INCLUDING, WITHOUT ";
		EULA += "LIMITATION, IMPLIED WARRANTIES OF TITLE, ";
		EULA += "NON-INFRINGEMENT, ACCURACY, MERCHANTABILITY, AND ";
		EULA += "FITNESS FOR A PARTICULAR PURPOSE, AND ANY WARRANTIES ";
		EULA += "THAT MAY ARISE FROM COURSE OF DEALING, COURSE OF ";
		EULA += "PERFORMANCE OR USAGE OF TRADE. THE COMPANY AND ITS ";
		EULA += "AFFILIATES, LICENSORS, SUPPLIERS, ADVERTISERS, SPONSORS, ";
		EULA += "PARTNERS AND AGENTS DO NOT WARRANT THAT YOUR USE OF ";
		EULA += "THE SERVICE WILL BE UNINTERRUPTED, ERROR-FREE OR ";
		EULA += "SECURE, THAT DEFECTS WILL BE CORRECTED, OR THAT THE ";
		EULA += "SERVICE, AND THE SERVER(S), SYSTEM(S) AND NETWORK(S) ON ";
		EULA += "WHICH THE SERVICE IS HOSTED AND/OR OPERATES, ARE FREE ";
		EULA += "OF VIRUSES OR OTHER HARMFUL COMPONENTS. YOU ";
		EULA += "ACKNOWLEDGE THAT YOU ARE RESPONSIBLE FOR OBTAINING ";
		EULA += "AND MAINTAINING ALL TELEPHONE, COMPUTER HARDWARE AND ";
		EULA += "OTHER EQUIPMENT NEEDED TO ACCESS AND USE THE SERVICE ";
		EULA += "AND ALL CHARGES AND FEES RELATED THERETO. YOU ASSUME ";
		EULA += "ALL RESPONSIBILITY AND RISK FOR YOUR USE OF THE SERVICE ";
		EULA += "AND YOUR RELIANCE THEREON. NO OPINION, ADVICE OR ";
		EULA += "STATEMENT OF THE COMPANY OR ITS PARENTS, SUBSIDIARIES, ";
		EULA += "AFFILIATES, LICENSORS, SUPPLIERS, ADVERTISERS, SPONSORS, ";
		EULA += "PARTNERS, AGENTS OR THIRD PARTY USERS, WHETHER MADE ";
		EULA += "ON THE SERVICE OR OTHERWISE, SHALL CREATE ANY ";
		EULA += "WARRANTY. YOUR USE OF THE SERVICE, THE CASH SHOP, ANY ";
		EULA += "CASH ITEMS, THE SOFTWARE AND ALL OTHER CONTENT ";
		EULA += "CONTAINED THEREIN IS ENTIRELY AT YOUR OWN RISK.";

		EULA += linebreak;

		EULA += "12. LIMITATION OF LIABILITY. NEITHER THE COMPANY NOR ";
		EULA += "ANY OF OUR PARENTS, SUBSIDIARIES, AFFILIATES, LICENSORS, ";
		EULA += "SUPPLIERS, ADVERTISERS, SPONSORS OR PARTNERS, NOR OUR ";
		EULA += "OR THEIR DIRECTORS, OFFICERS, EMPLOYEES, CONSULTANTS, ";
		EULA += "AGENTS, ATTORNEYS OR OTHER REPRESENTATIVES, ARE ";
		EULA += "RESPONSIBLE OR LIABLE FOR ANY INDIRECT, INCIDENTAL, ";
		EULA += "CONSEQUENTIAL, SPECIAL, EXEMPLARY, OR PUNITIVE ";
		EULA += "DAMAGES OF ANY KIND (INCLUDING, WITHOUT LIMITATION ";
		EULA += "DAMAGES FOR LOSS OF BUSINESS, LOSS OF DATA, LOSS OF ";
		EULA += "GOOD WILL OR LOST PROFITS), UNDER ANY CONTRACT, ";
		EULA += "NEGLIGENCE, STRICT LIABILITY OR OTHER THEORY ARISING ";
		EULA += "OUT OF OR RELATING IN ANY WAY TO THE SERVICE (INCLUDING, ";
		EULA += "WITHOUT LIMITATION THE SITE, THE GAMES, THE SOFTWARE ";
		EULA += "AND ALL OTHER CONTENT CONTAINED THEREIN), YOUR ";
		EULA += "ACCOUNT (INCLUDING, WITHOUT LIMITATION YOUR CASH ITEMS ";
		EULA += "OR NX), ANY LINKED SITE OR ANY PRODUCT OR SERVICE ";
		EULA += "PURCHASED THROUGH YOUR USE OF THE SERVICE, EVEN IF THE ";
		EULA += "COMPANY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH ";
		EULA += "DAMAGES. SOME JURISDICTIONS DO NOT ALLOW THE ";
		EULA += "EXCLUSION OR LIMITATION OF INCIDENTAL OR CONSEQUENTIAL ";
		EULA += "DAMAGES, SO THE ABOVE LIMITATION OR EXCLUSION MAY NOT ";
		EULA += "APPLY TO YOU. YOUR SOLE AND EXCLUSIVE REMEDY FOR ";
		EULA += "DISSATISFACTION WITH THE SERVICE, OR ANY PART THEREOF, ";
		EULA += "IS TO STOP USING THE SERVICE. THE MAXIMUM AGGREGATE ";
		EULA += "LIABILITY OF THE COMPANY FOR ALL DAMAGES, LOSSES AND ";
		EULA += "CAUSES OF ACTION, WHETHER IN CONTRACT, TORT (INCLUDING, ";
		EULA += "WITHOUT LIMITATION NEGLIGENCE), STRICT LIABILITY OR ";
		EULA += "OTHERWISE, SHALL BE THE LESSER OF TWENTY U.S. DOLLARS ";
		EULA += "OR THE TOTAL CUMULATIVE AMOUNT PAID BY YOU TO OBTAIN ";
		EULA += "CASH ITEMS.";

		EULA += linebreak;

		EULA += "13. INDEMNIFICATION. As a condition of your access to and use of ";
		EULA += "the Service and/or Site, you agree to indemnify, defend and hold the ";
		EULA += "Company, our parents, affiliates, subsidiaries, independent contractors, ";
		EULA += "licensors, suppliers, advertisers, partners, sublicensees and sponsors, ";
		EULA += "and our and their directors, officers, employees, consultants, agents, ";
		EULA += "attorneys and other representatives, harmless from and against any ";
		EULA += "and all claims, damages, losses, liabilities, costs (including reasonable ";
		EULA += "attorneys' fees) and other expenses that arise directly or indirectly out ";
		EULA += "of or from (i) your access to and use of the Service and/or Site and the ";
		EULA += "content therein, including, without limitation, any allegations that any ";
		EULA += "content you submit or transmit while using the Service infringes or ";
		EULA += "otherwise violates the copyright, trademark, trade secret or other ";
		EULA += "intellectual property rights, privacy or publicity rights or other rights of ";
		EULA += "any third person or party; (ii) your violation of this Agreement, the ";
		EULA += "Privacy Policy, any applicable law or the rights of any other person; ";
		EULA += "(iii) any dispute you have or claim to have with one or more users of ";
		EULA += "the Service and/or Site; (iv) the Company's resolution (if any) of any ";
		EULA += "dispute you have or claim to have with one or more users of the ";
		EULA += "Service and/or Site; (v) your improper authorization for the Company ";
		EULA += "to collect, use or disclose any data or content provided by you; and ";
		EULA += "(vi) any disclosures made with your permission (including, without ";
		EULA += "limitation, your consent that the Company disclose your personal ";
		EULA += "information and other information collected as set forth in our Privacy ";
		EULA += "Policy).";

		EULA += linebreak;

		EULA += "14. QUESTIONS/COMMENTS. The Service is provided by Nexon ";
		EULA += "America, Inc. If you have any questions, comments or complaints ";
		EULA += "regarding this Agreement or the Service, please contact our customer ";
		EULA += "service department by using the E-mail of Inquiry form on our website, ";
		EULA += "www.nexon.net. For inquiries related to business matters such as ";
		EULA += "licensing or merchandising, please contact us at biz@nexon.net. For ";
		EULA += "inquiries related to marketing, advertising, press, etc., please contact ";
		EULA += "our marketing department at press@nexon.net.";

		EULA += linebreak;

		EULA += "15. NOTICE FOR CALIFORNIA USERS. Under California Civil Code ";
		EULA += "Section 1789.3, users of the Service from California are entitled to the ";
		EULA += "following specific consumer rights notice: The Complaint Assistance ";
		EULA += "Unit of the Division of Consumer Services of the California ";
		EULA += "Department of Consumer Affairs may be contacted in writing at 400 R ";
		EULA += "Street, Suite 1080, Sacramento, California 95814, or by telephone at ";
		EULA += "(916) 445-1254 or (800) 952-5210.";

		EULA += linebreak;

		EULA += "16. LIQUIDATED DAMAGES. You acknowledge and agree that you ";
		EULA += "are not permitted for any reason or under any circumstances to hack, ";
		EULA += "attempt file decryption, crack encryptions, use cheats and/or memory ";
		EULA += "altering scripts or codes or utilize unauthorized third-party ";
		EULA += "manipulations programs, re-packer programs, game or client altering ";
		EULA += "software or client re-interpretation methods or codes to alter the play ";
		EULA += "experience or gain an unfair advantage in any of the Nexon games. ";
		EULA += "You also are prohibited from doing the following: (1) use any type of ";
		EULA += "hacks, bypass, decryptions (including but not limited to file decryption ";
		EULA += "software/methods), cheats and/or memory altering scripts or codes to ";
		EULA += "gain an unfair advantage over other users in any Nexon game; (2) ";
		EULA += "manipulate and/or alter the game client and/or server memory in ";
		EULA += "order to gain unearned and unjustified NX, in-game currency and/or ";
		EULA += "any other Items from any Nexon game; and (3) elevate your ranking ";
		EULA += "in any Nexon game fraudulently or by any wrongful or unauthorized ";
		EULA += "means. In addition you agree not to create, design, disseminate, link, ";
		EULA += "distribute, provide access to or use any third party programs, bots, ";
		EULA += "scripts, codes, bypass, memory scanning software, hacks or cheats ";
		EULA += "designed to alter the game play or to modify the code to play the ";
		EULA += "game differently than it was intended by means of programs not ";
		EULA += "permitted or authorized by Nexon, which include, but are not limited ";
		EULA += "to: hacks, private servers, cheat engines, memory altering scripts or ";
		EULA += "codes (including but not limited to disconnection hacks), auto clickers, ";
		EULA += "any type of bots and/or game cheats. Any use, dissemination or ";
		EULA += "design of such programs shall be deemed a material breach by you of ";
		EULA += "these Terms of Service for which you will be liable to Nexon for any ";
		EULA += "resulting damages. You agree that if you host or operate a private ";
		EULA += "server of a Nexon game or host or operate a hack site that sells hacks to ";
		EULA += "a Nexon game or if you distribute, disseminate, design, code, ";
		EULA += "moderate and/or provide free or paid access to private forums that sell ";
		EULA += "or distribute hacks, or assist in the design of any private servers or third ";
		EULA += "party programs (such as hacks) that alter the game play of any Nexon ";
		EULA += "game, you will have the unconditional obligation to pay Nexon ";
		EULA += "America Inc. the sum of One Hundred Thousand ($100,000) Dollars ";
		EULA += "for each such program (including patches) and versions thereof as ";
		EULA += "liquidated damages. You also agree that if you use such a program to ";
		EULA += "play any of the Nexon games, or engage in the use of a private server, ";
		EULA += "you mill have the unconditional obligation to pay Nexon America Inc. ";
		EULA += "the sum of One Thousand ($1,000) Dollars as liquidated damages for ";
		EULA += "each time you use such a program, which shall be due and payable ";
		EULA += "immediately upon notification to you of said infraction.";

		EULA += linebreak;

		EULA += "17. MISCELLANEOUS. This Agreement is governed by and ";
		EULA += "construed in accordance with the laws of the State of California, ";
		EULA += "United States of America, without regard to principles of conflicts of ";
		EULA += "laws that would result in the application of the law of a different ";
		EULA += "jurisdiction. You agree to submit to the exclusive jurisdiction of any ";
		EULA += "State or Federal court located in the County of Los Angeles, United ";
		EULA += "States of America, and waive any jurisdictional, venue or ";
		EULA += "inconvenient forum objections to such courts. If any provision of this ";
		EULA += "Agreement is found to be unlawful, void or for any reason ";
		EULA += "unenforceable, then that provision shall be deemed severable from ";
		EULA += "this Agreement and shall not affect the validity and enforceability of ";
		EULA += "any remaining provisions. Except as otherwise specifically provided ";
		EULA += "herein, this is the entire Agreement between us relating to the subject ";
		EULA += "matter herein and supersedes any and all prior or contemporaneous ";
		EULA += "written or oral Agreements between us with respect to such subject ";
		EULA += "matter. This Agreement is not assignable, transferable or ";
		EULA += "sublicensable by you except with the Company's prior written consent. ";
		EULA += "The failure of the Company to exercise or enforce any right or ";
		EULA += "provision of this Agreement shall not constitute a waiver of suds right ";
		EULA += "or provision. Any heading, caption or section title contained in this ";
		EULA += "Agreement is inserted only as a matter of convenience and in no way ";
		EULA += "defines or explains any section or provision hereof. The United ";
		EULA += "Nations Convention on the International Sale of Goods is explicitly ";
		EULA += "excluded from this Agreement. BY CLICKING ON THE \"I ACCEPT\" ";
		EULA += "BUTTON BELOW, YOU ACKNOWLEDGE THAT (1) YOU HAVE READ ";
		EULA += "AND REVIEWED THIS AGREEMENT IN ITS ENTIRETY, INCLUDING ";
		EULA += "THE END USER LICENSE AGREEMENT, PRIVACY POLICY AND THE ";
		EULA += "APPLICABLE GAME RULES AND POLICIES, (2) YOU AGREE TO ";
		EULA += "THE TERMS AND CONDITIONS OF THIS AGREEMENT AND (3) YOU ";
		EULA += "HAVE THE POWER, AUTHORITY AND LEGAL RIGHT TO ENTER ";
		EULA += "INTO THIS AGREEMENT.";
#pragma endregion

		text = Text(Text::Font::A11M, Text::Alignment::LEFT, Color::Name::BLACK, EULA, 340, true, 2);
		max_rows = text.height() / 300 + 1;

		int16_t slider_y = 77;

		slider = Slider(
				Slider::Type::LINE_PUNGA, Range<int16_t>(slider_y, slider_y + 305), 574, unit_rows, max_rows,
				[&](bool upwards)
				{
					int16_t shift = upwards ? -1 : 1;
					bool above = offset + shift >= 0;
					bool below = offset + shift <= max_rows - unit_rows;

					if (above && below)
					{
						offset += shift;
						update_accept(offset);
					}
				}
		);

		update_accept(offset);

		position = Point<int16_t>(0, 10);
		dimension = Texture(TOS).get_dimensions();
	}

	void UITermsOfService::draw(float inter) const
	{
		UIElement::draw(inter);

		int16_t range_min = 80;
		text.draw(position + Point<int16_t>(226, 84 - offset * 300), Range<int16_t>(range_min, range_min + 316));
		slider.draw(position);
	}

	Cursor::State UITermsOfService::send_cursor(bool clicked, Point<int16_t> cursorpos)
	{
		Point<int16_t> cursoroffset = cursorpos - position;

		if (slider.isenabled())
		{
			Cursor::State state = slider.send_cursor(cursoroffset, clicked);

			if (state != Cursor::State::IDLE)
				return state;
		}

		return UIElement::send_cursor(clicked, cursorpos);
	}

	UIElement::Type UITermsOfService::get_type() const
	{
		return TYPE;
	}

	Button::State UITermsOfService::button_pressed(uint16_t buttonid)
	{
		switch (buttonid)
		{
			case Buttons::OK:
				UI::get().emplace<UILoginWait>();

				TOSPacket().dispatch();
				break;
			case Buttons::CANCEL:
				deactivate();
				okhandler();
				break;
			default:
				break;
		}

		return Button::State::NORMAL;
	}

	void UITermsOfService::update_accept(uint16_t offset)
	{
		if (offset == max_rows - unit_rows)
			buttons[Buttons::OK]->set_state(Button::State::NORMAL);
		else
			buttons[Buttons::OK]->set_state(Button::State::DISABLED);
	}
}
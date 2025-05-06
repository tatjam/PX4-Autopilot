#ifndef UBAY_HITL_CAN_HPP
#define UBAY_HITL_CAN_HPP

#include <uORB/topics/ubay_hitl_can_to_hitl.h>
class MavlinkStreamUbayHitlCan : public MavlinkStream
{
public:
    static MavlinkStream* new_instance(Mavlink *mavlink)
    {
        return new MavlinkStreamUbayHitlCan(mavlink);
    }
    const char *get_name() const
    {
        return MavlinkStreamUbayHitlCan::get_name_static();
    }
    static const char *get_name_static()
    {
        return "UBAY_HITL_CAN";
    }
    static uint16_t get_id_static()
    {
        return MAVLINK_MSG_ID_IRPAS_SIM_CAN;
    }
    uint16_t get_id()
    {
        return get_id_static();
    }
    unsigned get_size()
    {
        return _ubay_hitl_can_to_hitl_sub.advertised() ? MAVLINK_MSG_ID_IRPAS_SIM_CAN_LEN + MAVLINK_NUM_NON_PAYLOAD_BYTES : 0;
    }

private:
		uORB::Subscription _ubay_hitl_can_to_hitl_sub{ORB_ID(ubay_hitl_can_to_hitl)};

    /* do not allow top copying this class */
    MavlinkStreamUbayHitlCan(MavlinkStreamUbayHitlCan &);
    MavlinkStreamUbayHitlCan& operator = (const MavlinkStreamUbayHitlCan &);

    explicit MavlinkStreamUbayHitlCan(Mavlink *mavlink) : MavlinkStream(mavlink)
    {}

		bool send() override
		{
			ubay_hitl_can_to_hitl_s ubay_hitl_can_to_hitl;

			if (_ubay_hitl_can_to_hitl_sub.update(&ubay_hitl_can_to_hitl)) {
				mavlink_irpas_sim_can_t can_msg{};

				can_msg.id = ubay_hitl_can_to_hitl.id;
				can_msg.data_len = ubay_hitl_can_to_hitl.data_size;
				for(int i = 0; i < can_msg.data_len; i++)
				{
					can_msg.data[i] = ubay_hitl_can_to_hitl.data[i];
				}
				//Send the message
				mavlink_msg_irpas_sim_can_send_struct(_mavlink->get_channel(), &can_msg);
				return true;
			}


			return false;
		}

};

#endif
